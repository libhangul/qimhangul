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
