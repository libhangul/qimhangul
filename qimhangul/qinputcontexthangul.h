#include <stack>
#include <wchar.h>

#include "hangulcomposer.h"
#include "candidatelist.h"

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

    virtual bool filterEvent( const QEvent *event);

    virtual void setFocus();
    virtual void unsetFocus();
    virtual void setMicroFocus( int x, int y, int w, int h, QFont *f = 0 );
    virtual void reset();

    friend class HangulComposer;

private:
    typedef enum {
	MODE_DIRECT,
	MODE_HANGUL
    } InputMode;

    void preeditUpdate(const QString &preeditString);
    void commit(const QString &preeditString);
    bool popupCandidateList();

    CandidateList *m_candidateList;
    HangulComposer m_composer;
    InputMode m_mode;
    QRect m_rect;
};

