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

#include <qapplication.h>
#include <qeventloop.h>
#include <qdialog.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qheader.h>
#include <qstatusbar.h>
#include <qlayout.h>
#include <qfont.h>
#include <qnamespace.h>

#include <hangul.h>

#include "candidatelist.h"

CandidateList::CandidateList(const HanjaList *list, int x, int y) :
    m_list(list),
    m_selected(false),
    m_size(0),
    m_currentPage(0),
    m_current(0),
    m_frame(NULL)
{
    if (m_list) {
	m_size = m_list->nitems;
	m_itemsPerPage = 9;
	
	m_frame = new QFrame(0, "CandidateList",
			    Qt::WType_Dialog | Qt::WStyle_Customize |
			    Qt::WStyle_NoBorder | Qt::WX11BypassWM);
	m_frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
	m_frame->setLineWidth(1);

	QBoxLayout *vlayout = new QVBoxLayout(m_frame, 5, 3);
    
	m_key = new QLabel(m_frame);
	QLabel *colon = new QLabel(":", m_frame);
	m_comment = new QLabel(m_frame);
	QBoxLayout *hlayout = new QHBoxLayout(vlayout, 3);
	hlayout->addWidget(m_key);
	hlayout->addWidget(colon);
	hlayout->addWidget(m_comment);
	hlayout->addStretch(0);

	m_listview = new QListView(m_frame);
	m_listview->addColumn("no");
	m_listview->addColumn("char");
	m_listview->addColumn("comment");
	m_listview->setAllColumnsShowFocus(true);
	m_listview->header()->hide();
	vlayout->addWidget(m_listview);

	QFont font(m_listview->font());
	if (font.pointSize() < 0)
	    font.setPixelSize(font.pixelSize() * 15 / 10);
	else
	    font.setPointSize(font.pointSize() * 15 / 10);
	m_listview->setFont(font);

	for (int i = 0; i < m_itemsPerPage; i++) {
	    (void)new QListViewItem(m_listview, "", "", "");
	}

	m_statusbar = new QStatusBar(m_frame);
	vlayout->addWidget(m_statusbar);

	updateList();
	updateCursor();

	m_frame->move(x, y);
	m_frame->show();
    }
}

CandidateList::~CandidateList()
{
    if (m_frame != NULL)
	delete m_frame;
}

QString CandidateList::getCandidate()
{
    return QString::fromUtf8(getCurrent());
}

bool CandidateList::filterEvent(const QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_K:
    case Qt::Key_BackSpace:
	prev();
	break;
    case Qt::Key_Down:
    case Qt::Key_J:
    case Qt::Key_Space:
	next();
	break;
    case Qt::Key_Left:
    case Qt::Key_H:
    case Qt::Key_Prior:
	prevPage();
	break;
    case Qt::Key_Right:
    case Qt::Key_L:
    case Qt::Key_Next:
    case Qt::Key_Tab:
	nextPage();
	break;
    case Qt::Key_Return:
	close();
	m_selected = true;
	return true;
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
	setCurrent(event->key() - Qt::Key_1);
	close();
	m_selected = true;
	return true;
    case Qt::Key_Escape:
	close();
	return true;
    default:
	break;
    }
    return false;
}

void CandidateList::close()
{
    delete m_frame;
    m_frame = NULL;
}

void CandidateList::move(int x, int y)
{
    if (m_frame != NULL)
	m_frame->move(x, y);
}

void CandidateList::updateList()
{
    QListViewItemIterator it(m_listview);
    for (int i = 0; i < m_itemsPerPage; i++) {
	QListViewItem *item = *it;
	if (i + m_currentPage < m_size) {
	    QString text;
	    text = QString::number(i + 1);
	    item->setText(0, text);

	    text = QString::fromUtf8(m_list->items[i + m_currentPage]->value);
	    item->setText(1, text);

	    text = QString::fromUtf8(m_list->items[i + m_currentPage]->comment);
	    item->setText(2, text);
	    item->setVisible(true);
	} else {
	    item->setText(0, "");
	    item->setText(1, "");
	    item->setText(2, "");
	    item->setVisible(false);
	}

	++it;
    }
}

void CandidateList::updateCursor()
{
    m_key->setText(QString::fromUtf8(m_list->items[m_current]->value));
    m_comment->setText(QString::fromUtf8(m_list->items[m_current]->comment));

    QListViewItemIterator it(m_listview);
    it += m_current - m_currentPage;
    m_listview->setCurrentItem(*it);

    QString statusText;
    m_statusbar->message(statusText.sprintf("%d/%d", m_current + 1, m_size));
}

void
CandidateList::prev()
{
    if (m_current > 0)
	m_current--;

    if (m_current < m_currentPage) {
	m_currentPage -= m_itemsPerPage;
	updateList();
    }
    updateCursor();
}

void
CandidateList::next()
{
    if (m_current < m_size - 1)
	m_current++;

    if (m_current >= m_currentPage + m_itemsPerPage) {
	m_currentPage += m_itemsPerPage;
	updateList();
    }
    updateCursor();
}

void
CandidateList::prevPage()
{
    if (m_currentPage - m_itemsPerPage >= 0) {
	m_current -= m_itemsPerPage;
	if (m_current < 0)
	    m_current = 0;
	m_currentPage -= m_itemsPerPage;
	updateList();
    }
    updateCursor();
}

void
CandidateList::nextPage()
{
    if (m_currentPage + m_itemsPerPage < m_size) {
	m_current += m_itemsPerPage;
	if (m_current > m_size - 1)
	    m_current = m_size - 1;
	m_currentPage += m_itemsPerPage;
	updateList();
    }
    updateCursor();
}

const char*
CandidateList::getCurrent()
{
    return m_list->items[m_current]->value;
}

void
CandidateList::setCurrent(int index)
{
    if (m_currentPage + index < m_size) {
	m_current = m_currentPage + index;
    }
}

const char*
CandidateList::getNth(int index)
{
    index += m_currentPage;
    if (index < 0 && index >= m_size)
	return NULL;

    return m_list->items[index]->value;
}
