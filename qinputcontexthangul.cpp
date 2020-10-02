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

#include <QDebug>
#include <QInputMethodEvent>
#include <QTextCodec>
#include <QTextFormat>
#include <QWidget>
#include <QWindow>
#include <QRect>
#include <QCoreApplication>
#include <QGuiApplication>

#include <hangul.h>
#include "qinputcontexthangul.h"

HanjaTable* QInputContextHangul::hanjaTable = NULL;

static inline QString ucsToQString(const ucschar *ucs);

static bool
onTransition(HangulInputContext* /* hic */,
	     ucschar /* c */, const ucschar *str, void* /* data */)
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

QInputContextHangul::QInputContextHangul(const QStringList& paramList) :
    m_candidateList(NULL),
    m_mode(MODE_DIRECT),
    m_focusObject(nullptr)
{
    QString keyboard;
    if (paramList.isEmpty()) {
        keyboard = "2";
    } else {
        keyboard = paramList.front();
    }
    m_hic = hangul_ic_new(keyboard.toUtf8());
    hangul_ic_connect_callback(m_hic, "transition", (void*)onTransition, NULL);
}

QInputContextHangul::~QInputContextHangul()
{
    delete m_candidateList;

    if (m_hic != NULL)
	hangul_ic_delete(m_hic);
}

bool
QInputContextHangul::isValid() const
{
    return true;
}

/**
 * @brief 현재 조합중인 한글을 취소하고 없애는 함수
 */
void QInputContextHangul::reset()
{
    // qDebug() << "QInputContextHangul::reset";

    if (m_candidateList != NULL && m_candidateList->isVisible()) {
	m_candidateList->close();
    }

    hangul_ic_reset(m_hic);

    QInputMethodEvent e;
    sendEvent(m_focusObject, &e);
}

/**
 * @brief 현재 조합중인 한글을 완성하여 commit해 주는 함수
 */
void
QInputContextHangul::commit()
{
    //qDebug() << "QInputContextHangul::commit";

    if (m_candidateList != NULL && m_candidateList->isVisible()) {
        m_candidateList->close();
    }

    if (m_focusObject == nullptr) {
        return;
    }

    const ucschar *flushed = hangul_ic_flush(m_hic);
    QString commitString = ucsToQString(flushed);
    if (commitString.isEmpty()) {
        return;
    }

    // flush된 후이므로 preedit string이 없다.
    QInputMethodEvent e;
    e.setCommitString(commitString);
    sendEvent(m_focusObject, &e);
}

QString QInputContextHangul::getPreeditString() const
{
    return ucsToQString(hangul_ic_get_preedit_string(m_hic));
}

QList<QInputMethodEvent::Attribute>
QInputContextHangul::getPreeditAttrs(const QString& preeditString)
{
    QList<QInputMethodEvent::Attribute> attrs;

    if (m_focusObject == nullptr) {
        return attrs;
    }

    QWidget* widget = qobject_cast<QWidget*>(m_focusObject);
    if (widget == NULL) {
        return attrs;
    }

    const QPalette& palette = widget->palette();

    QTextCharFormat format;
    format.setBackground(palette.color(QPalette::Text));
    format.setForeground(palette.color(QPalette::Window));
    attrs += QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat,
            0, preeditString.length(), format);

    return attrs;
}

QString QInputContextHangul::getCommitString() const
{
    return ucsToQString(hangul_ic_get_commit_string(m_hic));
}

void QInputContextHangul::updatePreedit(const QString &str)
{
    if (m_focusObject == nullptr) {
        return;
    }

    QList<QInputMethodEvent::Attribute> attrs;

    QWidget* widget = qobject_cast<QWidget*>(m_focusObject);
    if (widget != NULL) {
        const QPalette& palette = widget->palette();

        QTextCharFormat format;
        format.setBackground(palette.color(QPalette::Text));
        format.setForeground(palette.color(QPalette::Window));
        attrs += QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat,
                0, str.length(), format);
    }

    QInputMethodEvent e(str, attrs);
    sendEvent(m_focusObject, &e);
}

void QInputContextHangul::commit(const QString &str)
{
    if (m_focusObject == nullptr) {
        return;
    }

    QInputMethodEvent e;
    e.setCommitString(str);
    sendEvent(m_focusObject, &e);
}

bool QInputContextHangul::isTriggerKey(const QKeyEvent *event)
{
    return (event->key() == Qt::Key_Space && (event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) ||
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
	updatePreedit(str);
    }
    return ret;
}

bool QInputContextHangul::popupCandidateList()
{
    const ucschar *text = hangul_ic_get_preedit_string(m_hic);
    if (text != NULL && *text != 0) {
	QString str;
	str += QChar(text[0]);
	HanjaList *list = hanja_table_match_suffix(hanjaTable, str.toUtf8());

	if (m_candidateList == NULL)
	    m_candidateList = new CandidateList();

	QPoint p(0, 0);

        QWidget* focusWidget = m_focusObject ? qobject_cast<QWidget*>(m_focusObject) : nullptr;
        if (focusWidget != NULL) {
            QWidget* focusWindow = focusWidget->window();
            QRect cursorRect = qGuiApp->inputMethod()->cursorRectangle().toRect();
            p = focusWindow->mapToGlobal(cursorRect.bottomRight());
        }

	m_candidateList->open(list, p.x(), p.y());
    }

    return false;
}

bool QInputContextHangul::filterEvent(const QEvent *event)
{
    if (event->type() != QEvent::KeyPress)
	return false;

    const QKeyEvent *keyevent = static_cast<const QKeyEvent*>(event);
    if (m_candidateList != NULL && m_candidateList->isVisible()) {
	if (m_candidateList->filterEvent(keyevent)) {
	    if (m_candidateList->isSelected()) {
		hangul_ic_reset(m_hic);
		QString candidate(m_candidateList->getCandidate());
		commit(candidate);
	    }
	    m_candidateList->close();
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
            commit();
	    m_mode = MODE_DIRECT;
	}
	setModeInfo(m_mode);

	return true;
    }

    if (isCandidateKey(keyevent)) {
	return popupCandidateList();
    }

    if (keyevent->modifiers() & Qt::ControlModifier ||
	keyevent->modifiers() & Qt::AltModifier ||
	keyevent->modifiers() & Qt::MetaModifier) {
        commit();
	return false;
    }

    if (m_mode == MODE_HANGUL) {
	QString text = keyevent->text();
	if (keyevent->modifiers() & Qt::ShiftModifier)
	    text = text.toUpper();
	else
	    text = text.toLower();

	int ascii = 0;
	if (!text.isEmpty())
	    ascii = text[0].unicode();
	bool ret = hangul_ic_process(m_hic, ascii);

        QString preeditString = getPreeditString();
        QList<QInputMethodEvent::Attribute> attrs = getPreeditAttrs(preeditString);

        QInputMethodEvent e(preeditString, attrs);

        QString commitString = getCommitString();
        if (!commitString.isEmpty())
            e.setCommitString(commitString);

        sendEvent(m_focusObject, &e);

	return ret;
    }

    return false;
}

void
QInputContextHangul::setFocusObject(QObject* object)
{
    m_focusObject = object;

    if (m_focusObject == nullptr) {
        setModeInfo(MODE_NONE);
    } else {
        setModeInfo(m_mode);
    }

    QPlatformInputContext::setFocusObject(object);
}

void
QInputContextHangul::sendEvent(QObject* object, QInputMethodEvent* event)
{
    if (object == nullptr) {
        return;
    }

    QCoreApplication::sendEvent(m_focusObject, event);
}
