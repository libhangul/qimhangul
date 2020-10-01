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

#include <hangul.h>
#include "candidatelist.h"

#include <QString>
#include <QRect>

#include <qpa/qplatforminputcontext.h>

class QEvent;
class QFont;

class QInputContextHangul : public QPlatformInputContext {
    Q_OBJECT

public:
    QInputContextHangul(const QStringList& paramList);
    ~QInputContextHangul();

    bool isValid() const override;

    bool filterEvent(const QEvent* event) override;

    void setFocusObject(QObject* object);

    virtual void setFocus();
    virtual void unsetFocus();
    virtual void setMicroFocus( int x, int y, int w, int h, QFont *f = 0 );
    virtual void reset();
    virtual bool isComposing() const;

    static HanjaTable* hanjaTable;

    friend class HangulComposer;

private:
    typedef enum {
	MODE_NONE,
	MODE_DIRECT,
	MODE_HANGUL
    } InputMode;

    bool isTriggerKey(const QKeyEvent *event);
    bool isCandidateKey(const QKeyEvent *event);
    QString getPreeditString() const;
    QString getCommitString() const;
    void updatePreedit(const QString &str);
    void commit(const QString &str);
    bool backspace();
    bool popupCandidateList();
    void setModeInfo(int mode);

    CandidateList *m_candidateList;
    HangulInputContext *m_hic;
    InputMode m_mode;
    QRect m_rect;
    QObject* m_focusObject;
};
