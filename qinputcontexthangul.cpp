/* qimhangul - Qt input module for hangul
 * Copyright (C) 2004 Choe Hwanjin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <ctype.h>

#include <qapplication.h>
#include <qstring.h>
#include <qevent.h>
#include <qtextcodec.h>

#include <hangul.h>
#include "qinputcontexthangul.h"

HanjaTable* QInputContextHangul::hanjaTable = NULL;

static inline QString ucsToQString(const ucschar *ucs);

static bool
filter(ucschar *str,
       ucschar /*cho*/, ucschar /*jung*/, ucschar /*jong*/, void* /*data*/)
{
    QTextCodec *codec = QTextCodec::codecForLocale();
    QString s = ucsToQString(str);
    return codec->canEncode(s);
}

static inline QString ucsToQString(const ucschar *ucs)
{
    QString str;

    if (ucs != NULL) {
	while (*ucs != 0)
	    str += QChar(*ucs++);
    }
    return str;
}

QInputContextHangul::QInputContextHangul(HangulKeyboardType keyboard) :
    m_candidateList(NULL),
    m_mode(MODE_DIRECT),
    m_rect(0, 0, 0, 0)
{
    m_hic = hangul_ic_new(keyboard);
    hangul_ic_set_filter(m_hic, filter, NULL);
}

QInputContextHangul::~QInputContextHangul()
{
    if (m_hic != NULL)
	hangul_ic_delete(m_hic);
}

QString QInputContextHangul::identifierName()
{
    return "Hangul";
}

QString QInputContextHangul::language()
{
    return "Hangul";
}

void QInputContextHangul::setFocus()
{
    setModeInfo(m_mode);
}

void QInputContextHangul::unsetFocus()
{
    if (m_candidateList != NULL) {
	delete m_candidateList;
	m_candidateList = NULL;
    }
    reset();

    setModeInfo(MODE_NONE);
}

void QInputContextHangul::setMicroFocus(int x, int y, int w, int h, QFont* /*f*/)
{
    m_rect.setRect(x, y, w, h);
    if (m_candidateList != NULL) {
	m_candidateList->move(x, y + h);
    }
}

void QInputContextHangul::reset()
{
    const ucschar *flushed = hangul_ic_flush(m_hic);

    // we do not send preedit update IMEvent
    // because commit() send IMEnd event and it remove preedit string
    // QString preeditString = getPreeditString();
    // updatePreedit(preeditString);

    QString commitString = ucsToQString(flushed);
    if (!commitString.isEmpty()) {
	commit(commitString);
    } else {
	if (isComposing())
	    sendIMEvent(QEvent::IMEnd, QString::null);
    }
}

QString QInputContextHangul::getPreeditString()
{
    return ucsToQString(hangul_ic_get_preedit_string(m_hic));
}

QString QInputContextHangul::getCommitString()
{
    return ucsToQString(hangul_ic_get_commit_string(m_hic));
}

void QInputContextHangul::updatePreedit(const QString &str)
{
    if (!isComposing()) {
	sendIMEvent(QEvent::IMStart);
    }
    sendIMEvent(QEvent::IMCompose, str, str.length());
}

void QInputContextHangul::commit(const QString &str)
{
    if (!isComposing()) {
	sendIMEvent(QEvent::IMStart);
    }
    sendIMEvent(QEvent::IMEnd, str);
}

bool QInputContextHangul::isTriggerKey(const QKeyEvent *event)
{
    return (event->key() == Qt::Key_Space && (event->state() & Qt::ShiftButton) == Qt::ShiftButton) ||
	   (event->key() == Qt::Key_Hangul);
}

bool QInputContextHangul::isCandidateKey(const QKeyEvent *event)
{
    return (event->key() == Qt::Key_Hangul_Hanja) ||
	   (event->key() == Qt::Key_F9);
}

bool QInputContextHangul::backspace()
{
    bool ret = hangul_ic_backspace(m_hic);
    if (ret) {
	QString str = getPreeditString();
	if (!isComposing())
	    sendIMEvent(QEvent::IMStart);

	if (str.length() > 0)
	    sendIMEvent(QEvent::IMCompose, str, str.length());
	else
	    sendIMEvent(QEvent::IMEnd, QString::null);
    }
    return ret;
}

bool QInputContextHangul::popupCandidateList()
{
    const ucschar *text = hangul_ic_get_preedit_string(m_hic);
    if (text != NULL && *text != 0) {
	QString str;
	str += QChar(text[0]);
	HanjaList *list = hanja_table_match_suffix(hanjaTable, str.utf8());
	m_candidateList = new CandidateList(list,
					    m_rect.left(), m_rect.bottom());
    }

    return false;
}

bool QInputContextHangul::filterEvent(const QEvent *event)
{
    if (event->type() == QEvent::KeyRelease)
	return false;

    const QKeyEvent *keyevent = static_cast<const QKeyEvent*>(event);
    if (m_candidateList != NULL) {
	if (m_candidateList->filterEvent(keyevent)) {
	    if (m_candidateList->isSelected()) {
		hangul_ic_reset(m_hic);
		QString candidate(m_candidateList->getCandidate());
		commit(candidate);
	    }
	    delete m_candidateList;
	    m_candidateList = NULL;
	}
	return true;
    }

    if (keyevent->key() == Qt::Key_Shift)
	return false;

    if (keyevent->key() == Qt::Key_Backspace)
	return backspace();

    if (isTriggerKey(keyevent)) {
	if (m_mode == MODE_DIRECT) {
	    m_mode = MODE_HANGUL;
	} else {
	    reset();
	    m_mode = MODE_DIRECT;
	}
	setModeInfo(m_mode);

	return true;
    }

    if (isCandidateKey(keyevent)) {
	return popupCandidateList();
    }

    if (keyevent->state() & Qt::ControlButton ||
	keyevent->state() & Qt::AltButton ||
	keyevent->state() & Qt::MetaButton) {
	reset();
	return false;
    }

    if (m_mode == MODE_HANGUL) {
	int ascii = keyevent->ascii();
	if (keyevent->state() & Qt::ShiftButton)
	    ascii = toupper(ascii);
	else
	    ascii = tolower(ascii);
	
	bool ret = hangul_ic_process(m_hic, ascii);

	QString commitString = getCommitString();
	if (!commitString.isEmpty())
	    commit(commitString);

	QString preeditString = getPreeditString();
	if (!preeditString.isEmpty())
	    updatePreedit(preeditString);

	return ret;
    }

    return false;
}
