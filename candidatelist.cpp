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

#include <QBoxLayout>
#include <QFont>
#include <QFrame>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMenu>
#include <QTreeWidget>

#include <hangul.h>

#include "candidatelist.h"

CandidateList::CandidateList() :
    m_list(NULL),
    m_selected(false),
    m_size(0),
    m_currentPage(0),
    m_current(0),
    m_frame(NULL)
{
    m_itemsPerPage = 9;

    m_frame = new QFrame(NULL, Qt::Dialog | Qt::X11BypassWindowManagerHint);
    m_frame->setFrameStyle(QFrame::Panel | QFrame::Plain);

    QBoxLayout *vlayout = new QVBoxLayout(m_frame);
    vlayout->setSpacing(0);
    vlayout->setMargin(3);

    QGridLayout* glayout = new QGridLayout();
    glayout->setSpacing(0);
    glayout->setMargin(0);
    glayout->setColumnStretch(2, 1);
    vlayout->addLayout(glayout);

    m_indexes = new QLabel*[m_itemsPerPage];
    m_values = new QLabel*[m_itemsPerPage];
    m_comments = new QLabel*[m_itemsPerPage];

    for (int i = 0; i < m_itemsPerPage; i++) {
        m_indexes[i] = new QLabel(QString::asprintf(" %d ", i + 1), m_frame);
	m_indexes[i]->setAutoFillBackground(true);
	m_indexes[i]->setMargin(3);
	glayout->addWidget(m_indexes[i], i, 0);

	m_values[i] = new QLabel(m_frame);
	m_values[i]->setAutoFillBackground(true);
	m_values[i]->setMargin(3);
	glayout->addWidget(m_values[i], i, 1);

	m_comments[i] = new QLabel(m_frame);
	m_comments[i]->setWordWrap(true);
	m_comments[i]->setAutoFillBackground(true);
	m_comments[i]->setMargin(3);
	glayout->addWidget(m_comments[i], i, 2);
    }

    QFont font(m_values[0]->font());
    if (font.pointSize() < 0)
	font.setPixelSize(font.pixelSize() * 15 / 10);
    else
	font.setPointSize(font.pointSize() * 15 / 10);

    for (int i = 0; i < m_itemsPerPage; i++) {
	m_values[i]->setFont(font);
    }

    m_statusbar = new QLabel(m_frame);
    m_statusbar->setAlignment(Qt::AlignRight);
    vlayout->addWidget(m_statusbar);
}

CandidateList::~CandidateList()
{
    delete[] m_indexes;
    delete[] m_values;
    delete[] m_comments;

    delete m_frame;
}

void CandidateList::open(const HanjaList* list, int x, int y)
{
    m_list = list;
    m_selected = false;
    m_currentPage = 0;
    m_current = 0;

    if (m_list != NULL) {
	m_size = hanja_list_get_size(m_list);

	updateList();
	updateCursor();

	m_frame->move(x, y);
	m_frame->show();
    }
}

bool CandidateList::isVisible() const
{
    if (m_frame != NULL)
	return m_frame->isVisible();

    return false;
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
    case Qt::Key_Backspace:
	prev();
	break;
    case Qt::Key_Down:
    case Qt::Key_J:
    case Qt::Key_Space:
	next();
	break;
    case Qt::Key_Left:
    case Qt::Key_H:
    case Qt::Key_PageUp:
	prevPage();
	break;
    case Qt::Key_Right:
    case Qt::Key_L:
    case Qt::Key_PageDown:
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
    if (m_frame != NULL)
	m_frame->hide();
}

void CandidateList::move(int x, int y)
{
    if (m_frame != NULL)
	m_frame->move(x, y);
}

void CandidateList::updateList()
{
    for (int i = 0; i < m_itemsPerPage; ++i) {
	if (i + m_currentPage < m_size) {
	    QString text;
	    const char *value = hanja_list_get_nth_value(m_list,
						     i + m_currentPage);
	    const char *comment = hanja_list_get_nth_comment(m_list,
						     i + m_currentPage);

	    m_values[i]->setText(QString::fromUtf8(value));
	    m_comments[i]->setText(QString::fromUtf8(comment));
	} else {
	    m_values[i]->setText("");
	    m_comments[i]->setText("");
	}
    }
}

void CandidateList::updateCursor()
{
    int c = m_current - m_currentPage;

    for (int i = 0; i < m_itemsPerPage; i++) {
	if (i == c) {
	    m_indexes[i]->setForegroundRole(QPalette::HighlightedText);
	    m_indexes[i]->setBackgroundRole(QPalette::Highlight);
	    m_values[i]->setForegroundRole(QPalette::HighlightedText);
	    m_values[i]->setBackgroundRole(QPalette::Highlight);
	    m_comments[i]->setForegroundRole(QPalette::HighlightedText);
	    m_comments[i]->setBackgroundRole(QPalette::Highlight);
	} else {
	    m_indexes[i]->setForegroundRole(QPalette::Foreground);
	    m_indexes[i]->setBackgroundRole(QPalette::Background);
	    m_values[i]->setForegroundRole(QPalette::Foreground);
	    m_values[i]->setBackgroundRole(QPalette::Background);
	    m_comments[i]->setForegroundRole(QPalette::Foreground);
	    m_comments[i]->setBackgroundRole(QPalette::Background);
	}
    }

    QString statusText;
    statusText = QString::asprintf("%d/%d", m_current + 1, m_size);
    m_statusbar->setText(statusText);
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
    return hanja_list_get_nth_value(m_list, m_current);
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

    return hanja_list_get_nth_value(m_list, index);
}
