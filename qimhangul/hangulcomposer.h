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

#include <string>
#include <stack>
#include <wchar.h>

#include "hangul.h"

namespace hangul {

typedef std::basic_string<wchar_t> widestring; 

class Buffer
{
public:
    Buffer();
    ~Buffer();

    widestring getPreeditString();
    bool backspace();

    bool hasChoseong() { return m_choseong != 0; }
    bool hasJungseong() { return m_jungseong != 0; }
    bool hasJongseong() { return m_jongseong != 0; }
    void clear();

    void push(wchar_t ch);
    wchar_t peek();
    wchar_t pop();
    int size() { return m_stack.size(); }

    wchar_t m_choseong;
    wchar_t m_jungseong;
    wchar_t m_jongseong;

private:
    std::stack<wchar_t> m_stack;
};

class ComposerBase
{
public:
    typedef enum {
	HANGUL2,
	HANGUL3F,
	HANGUL390,
	HANGUL32
    } Keyboard;

    typedef enum {
	FILTER2,
	FILTER3
    } FilterType;

    ComposerBase(Keyboard keyboard);
    virtual ~ComposerBase();

    virtual void preeditUpdate(widestring &text) = 0;
    virtual void commit(widestring &text) = 0;

    virtual bool filter(int ascii);
    void reset();
    void clear();
    bool backspace();
    widestring getPreeditString();

protected:
    void syllabicate();
    bool complete(wchar_t ch);
    wchar_t translateToJamo(int ascii);
    bool filter2(wchar_t ch);
    bool filter3(wchar_t ch);

private:
    int m_filterType;
    const wchar_t *m_keyboard;
    Buffer m_buf;
};

inline Buffer::Buffer()
{
    clear();
}

inline Buffer::~Buffer()
{
}

inline void Buffer::clear()
{
    while (!m_stack.empty())
	m_stack.pop();

    m_choseong = 0;
    m_jungseong = 0;
    m_jongseong = 0;
}

inline void Buffer::push(wchar_t ch)
{
    if (hangul_is_choseong(ch)) {
	m_choseong = ch;
    } else if (hangul_is_jungseong(ch)) {
	m_jungseong = ch;
    } else {
	m_jongseong = ch;
    }
    m_stack.push(ch);
}

inline wchar_t Buffer::peek()
{
    if (m_stack.empty())
	return 0;
    else
	return m_stack.top();
}

inline wchar_t Buffer::pop()
{
    wchar_t ch = m_stack.top();
    m_stack.pop();
    return ch;
}

inline widestring Buffer::getPreeditString()
{
    wchar_t ch = 0;
    widestring preeditString = L"";

    if (m_choseong) {
	if (m_jungseong) {
	    /* have cho, jung, jong or no jong */
	    ch = hangul_jamo_to_syllable(m_choseong, m_jungseong, m_jongseong);
	    preeditString += ch;
	} else {
	    if (m_jongseong) {
		/* have cho, jong */
		ch = hangul_choseong_to_cjamo(m_choseong);
		preeditString += ch;
		ch = hangul_jongseong_to_cjamo(m_jongseong);
		preeditString += ch;
	    } else {
		/* have cho */
		ch = hangul_choseong_to_cjamo(m_choseong);
		preeditString += ch;
	    }
	}
    } else {
	if (m_jungseong) {
	    if (m_jongseong) {
		/* have jung, jong */
		ch = hangul_jungseong_to_cjamo(m_jungseong);
		preeditString += ch;
		ch = hangul_jongseong_to_cjamo(m_jongseong);
		preeditString += ch;
	    } else {
		/* have jung */
		ch = hangul_jungseong_to_cjamo(m_jungseong);
		preeditString += ch;
	    }
	} else {
	    if (m_jongseong) {
		/* have jong */
		ch = hangul_jongseong_to_cjamo(m_jongseong);
		preeditString += ch;
	    } else {
		/* have nothing */
		;
	    }
	}
    }

    return preeditString;
}

inline bool Buffer::backspace()
{
    if (!m_stack.empty()) {
	wchar_t ch = pop();
	if (ch == 0)
	    return false;

	if (hangul_is_choseong(ch)) {
	    ch = peek();
	    m_choseong = hangul_is_choseong(ch) ? ch : 0;
	    printf("stack length: %d\n", m_stack.size());
	    return true;
	}
	if (hangul_is_jungseong(ch)) {
	    ch = peek();
	    m_jungseong = hangul_is_jungseong(ch) ? ch : 0;
	    printf("stack length: %d\n", m_stack.size());
	    return true;
	}
	if (hangul_is_jongseong(ch)) {
	    ch = peek();
	    m_jongseong = hangul_is_jongseong(ch) ? ch : 0;
	    printf("stack length: %d\n", m_stack.size());
	    return true;
	}
    }
    return false;
}

inline void ComposerBase::clear()
{
    m_buf.clear();
}

inline widestring ComposerBase::getPreeditString()
{
    return m_buf.getPreeditString();
}

} // namespace hangul
