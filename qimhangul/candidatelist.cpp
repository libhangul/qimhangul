#include <qapplication.h>
#include <qeventloop.h>
#include <qdialog.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qnamespace.h>

#include "candidatelist.h"
#include "candidatetable.h"

CandidateList::CandidateList(wchar_t ch, int x, int y) :
    m_selected(false),
    m_size(0),
    m_currentPage(0),
    m_current(0),
    m_data(0),
    m_listBox(NULL)
{
    int index = getTableIndex(ch);
    if (index >= 0) {
	m_data = candidate_table[index] + 1;
	for (m_size = 0; m_data[m_size].ch != 0; m_size++)
	    continue;
	m_itemsPerPage = 9;

	m_listBox = new QListBox(0, "CandidateList",
				Qt::WType_Dialog | Qt::WStyle_Customize |
				Qt::WStyle_NoBorder | Qt::WX11BypassWM),
	m_listBox->setFrameStyle( QFrame::Plain | QFrame::Box );
	m_listBox->setLineWidth(1);
	m_listBox->move(x, y);

	updateList();
	updateCursor();

	m_listBox->show();
    }
}

CandidateList::~CandidateList()
{
    if (m_listBox != NULL)
	delete m_listBox;
}

QChar CandidateList::getCandidate()
{
    return QChar(getCurrent());
}

bool CandidateList::filterEvent(const QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_K:
	prev();
	return true;
    case Qt::Key_Down:
    case Qt::Key_J:
	next();
	return true;
    case Qt::Key_Left:
    case Qt::Key_H:
    case Qt::Key_Prior:
    case Qt::Key_BackSpace:
	prevPage();
	return true;
    case Qt::Key_Right:
    case Qt::Key_L:
    case Qt::Key_Space:
    case Qt::Key_Next:
    case Qt::Key_Tab:
	nextPage();
	return true;
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
    delete m_listBox;
    m_listBox = NULL;
}

void CandidateList::updateList()
{
    m_listBox->clear();
    for (int i = m_currentPage;
	 i < m_currentPage + m_itemsPerPage && m_data[i].ch != 0;
	 i++) {
	QString text;
	text += QString::number(i - m_currentPage + 1);
	text += " ";
	text += QChar(m_data[i].ch);
	text += " ";
	text += QString::fromUtf8(m_data[i].comment);
	m_listBox->insertItem(text);
    }
}

void CandidateList::updateCursor()
{
    m_listBox->setSelected(m_current - m_currentPage, TRUE);
}

int CandidateList::getTableIndex(wchar_t ch)
{
    int first, last, mid;

    /* binary search */
    first = 0;
    last = sizeof(candidate_table) / sizeof(candidate_table[0]) - 1;
    while (first <= last) {
	mid = (first + last) / 2;

	if (ch == candidate_table[mid][0].ch)
	    return mid;

	if (ch < candidate_table[mid][0].ch)
	    last = mid - 1;
	else
	    first = mid + 1;
    }

    return -1;
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

unsigned short int
CandidateList::getCurrent()
{
    return m_data[m_current].ch;
}

void
CandidateList::setCurrent(int index)
{
    if (m_currentPage + index < m_size) {
	m_current = m_currentPage + index;
    }
}

unsigned short int
CandidateList::getNth(int n)
{
    n += m_currentPage;
    if (n < 0 && n >= m_size)
	return 0;

    m_current = n;
    return m_data[n].ch;
}
