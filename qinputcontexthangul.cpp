#include <ctype.h>

#include <qapplication.h>
#include <qstring.h>
#include <qevent.h>

#include "hangul.h"
#include "qinputcontexthangul.h"

HangulComposer::HangulComposer(hangul::ComposerBase::Keyboard keyboard, QInputContextHangul *context) :
    ComposerBase(keyboard),
    m_context(context)
{
}

QString HangulComposer::getPreeditQString(hangul::widestring &text)
{
    QChar ch(0);
    QString str;
    hangul::widestring::iterator iter;

    for (iter = text.begin(); iter != text.end(); ++iter) {
	ch = (uint)(*iter);
	str += ch;
    }
    return str;
}

void HangulComposer::preeditUpdate(hangul::widestring &text)
{
    QString preeditString = getPreeditQString(text);
    m_context->preeditUpdate(preeditString);
}

void HangulComposer::commit(hangul::widestring &text)
{
    QString preeditString = getPreeditQString(text);
    m_context->commit(preeditString);
}

QInputContextHangul::QInputContextHangul(hangul::ComposerBase::Keyboard keyboard) :
    m_candidateList(NULL),
    m_composer(keyboard, this),
    m_mode(MODE_DIRECT),
    m_rect(0, 0, 0, 0)
{
    qDebug("Hangul::");
}

QInputContextHangul::~QInputContextHangul()
{
    qDebug("Hangul::~");
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
    qDebug("Hangul::setFocus");
}

void QInputContextHangul::unsetFocus()
{
    if (m_candidateList != NULL) {
	delete m_candidateList;
	m_candidateList = NULL;
	m_composer.reset();
    }
    m_composer.reset();

    setModeInfo(MODE_NONE);

    qDebug("Hangul::unsetFocus");
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
    m_composer.reset();
    qDebug("Hangul::reset");
}

void QInputContextHangul::preeditUpdate(const QString &preeditString)
{
    if (!isComposing()) {
	sendIMEvent(QEvent::IMStart);
    }
    sendIMEvent(QEvent::IMCompose, preeditString, preeditString.length());
}

void QInputContextHangul::commit(const QString &preeditString)
{
    if (!isComposing()) {
	sendIMEvent(QEvent::IMStart);
    }
    sendIMEvent(QEvent::IMEnd, preeditString);
}

bool QInputContextHangul::popupCandidateList()
{
    hangul::widestring text = m_composer.getPreeditString();
    if (!text.empty()) {
	int index = CandidateList::getTableIndex(text[0]);
	if (index >= 0) {
	    m_candidateList = new CandidateList(index,
						m_rect.left(), m_rect.bottom());
	    return false;
	}
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
		m_composer.clear();
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

    if (keyevent->key() == Qt::Key_Backspace) {
	return m_composer.backspace();
    }

    if (keyevent->key() == Qt::Key_Space &&
	(keyevent->state() & Qt::ShiftButton) == Qt::ShiftButton) {
	if (m_mode == MODE_DIRECT) {
	    m_mode = MODE_HANGUL;
	} else {
	    m_composer.reset();
	    m_mode = MODE_DIRECT;
	}
	setModeInfo(m_mode);

	return true;
    }

    if (keyevent->key() == Qt::Key_F9) {
	return popupCandidateList();
    }

    if (m_mode == MODE_HANGUL) {
	return m_composer.filter(keyevent->ascii());
    }

    return false;
}
