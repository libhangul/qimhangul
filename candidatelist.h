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
    CandidateList(wchar_t ch, int x, int y);
    virtual ~CandidateList();

    bool filterEvent(const QKeyEvent *event);
    int size() { return m_size; }
    bool isClosed() { return m_listBox == NULL; }
    bool isSelected() { return m_selected; }
    QChar getCandidate();

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
    int getTableIndex(wchar_t ch);

    bool m_selected;
    int m_size;
    int m_itemsPerPage;
    int m_currentPage;
    int m_current;
    const CandidateItem *m_data;
    QListBox *m_listBox;
};

#endif /* _CANDIDATE_H_ */
