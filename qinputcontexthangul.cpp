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
    m_composer(keyboard, this),
    m_mode(MODE_DIRECT)
{
    qDebug("Hangul::");
}

QInputContextHangul::~QInputContextHangul()
{
    qDebug("Hangul::~");
}

QCString QInputContextHangul::identifierName()
{
    return "Hangul";
}

QCString QInputContextHangul::language()
{
    return "Hangul";
}

void QInputContextHangul::setFocus()
{
    qDebug("Hangul::setFocus");
}

void QInputContextHangul::unsetFocus()
{
    m_composer.reset();
    qDebug("Hangul::unsetFocus");
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
    if (preeditString.length() == 0) {
	if (isComposing()) {
	    sendIMEvent(QEvent::IMEnd, QString::null);
	}
    } else {
	if (!isComposing()) {
	    sendIMEvent(QEvent::IMStart);
	}
	sendIMEvent(QEvent::IMEnd, preeditString);
    }
}

bool QInputContextHangul::filterEvent(const QEvent *event)
{
    if (event->type() == QEvent::KeyRelease)
	return false;

    const QKeyEvent *keyevent = static_cast<const QKeyEvent*>(event);
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
	    m_mode = MODE_DIRECT;
	}
	return true;
    }

    if (m_mode == MODE_HANGUL) {
	return m_composer.filter(keyevent->ascii());
    }

    return false;
}
