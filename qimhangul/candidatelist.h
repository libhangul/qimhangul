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

#ifndef _CANDIDATE_H_
#define _CANDIDATE_H_

#include <wchar.h>
#include <qstring.h>

typedef struct _CandidateItem CandidateItem;

struct _CandidateItem {
    unsigned short int ch;
    const char *comment;
};

class QListBox;
class QKeyEvent;
class CandidateList {
public:
    CandidateList(int index, int x, int y);
    virtual ~CandidateList();

    static int getTableIndex(wchar_t ch);

    bool filterEvent(const QKeyEvent *event);
    bool isSelected() { return m_selected; }
    QChar getCandidate();

    void move(int x, int y);

private:
    void prev();
    void next();
    void prevPage();
    void nextPage();
    unsigned short int getCurrent();
    unsigned short int getNth(int index);
    void setCurrent(int index);
    void setPosition(int x, int y);
    void close();
    void updateList();
    void updateCursor();

    bool m_selected;
    int m_size;
    int m_itemsPerPage;
    int m_currentPage;
    int m_current;
    const CandidateItem *m_data;
    QListBox *m_listBox;
};

#endif /* _CANDIDATE_H_ */
