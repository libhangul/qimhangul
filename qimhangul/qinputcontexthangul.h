#include <stack>
#include <wchar.h>

#include "hangulcomposer.h"

#include <qinputcontext.h>

class QInputContextHangul;
class HangulComposer : public hangul::ComposerBase {
public:
    HangulComposer(hangul::ComposerBase::Keyboard keyboard, QInputContextHangul *context);

    virtual void preeditUpdate(hangul::widestring &text);
    virtual void commit(hangul::widestring &text);

private:
    QString getPreeditQString(hangul::widestring &text);
    QInputContextHangul *m_context;
};

class QInputContextHangul : public QInputContext {
public:
    QInputContextHangul(hangul::ComposerBase::Keyboard keyboard);
    ~QInputContextHangul();
    
    virtual QCString identifierName();
    virtual QCString language();

    bool filterEvent( const QEvent *event);

    void setFocus();
    void unsetFocus();
    void reset();

    friend class HangulComposer;

private:
    typedef enum {
	MODE_DIRECT,
	MODE_HANGUL
    } InputMode;

    void preeditUpdate(const QString &preeditString);
    void commit(const QString &preeditString);

    HangulComposer m_composer;
    InputMode m_mode;
};

