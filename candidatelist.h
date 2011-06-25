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

#include <QString>
#include <hangul.h>

class QLabel;
class QFrame;
class QTreeWidget;
class QStatusBar;
class QKeyEvent;
class QMenu;
class QAction;

class CandidateList {
public:
    CandidateList();
    virtual ~CandidateList();

    void open(const HanjaList* list, int x, int y);
    void move(int x, int y);
    void close();

    bool isVisible() const;

    bool filterEvent(const QKeyEvent *event);
    bool isSelected() { return m_selected; }
    QString getCandidate();

private:
    void prev();
    void next();
    void prevPage();
    void nextPage();
    const char* getCurrent();
    const char* getNth(int index);
    void setCurrent(int index);
    void setPosition(int x, int y);
    void updateList();
    void updateCursor();

    const HanjaList *m_list;
    bool m_selected;
    int m_size;
    int m_itemsPerPage;
    int m_currentPage;
    int m_current;

    QFrame *m_frame;
    QLabel **m_indexes;
    QLabel **m_values;
    QLabel **m_comments;
    QLabel *m_statusbar;
};

#endif /* _CANDIDATE_H_ */
