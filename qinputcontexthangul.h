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
    
    virtual QString identifierName();
    virtual QString language();

    virtual bool filterEvent( const QEvent *event);

    virtual void setFocus();
    virtual void unsetFocus();
    virtual void setMicroFocus( int x, int y, int w, int h, QFont *f = 0 );
    virtual void reset();

    friend class HangulComposer;

private:
    typedef enum {
	MODE_NONE,
	MODE_DIRECT,
	MODE_HANGUL
    } InputMode;

    void preeditUpdate(const QString &preeditString);
    void commit(const QString &preeditString);
    bool popupCandidateList();
    void setModeInfo(int mode);

    CandidateList *m_candidateList;
    HangulComposer m_composer;
    InputMode m_mode;
    QRect m_rect;
};

#if !defined(Q_WS_X11)
inline void QInputContextHangul::setModeInfo(int /*mode*/)
{
}
#endif
