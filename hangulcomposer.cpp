#include <ctype.h>
#include "hangul.h"

#include "keyboard.h"
#include "hangulcomposer.h"

using namespace hangul;

ComposerBase::ComposerBase(Keyboard keyboard)
{
    switch (keyboard) {
    case HANGUL2:
	m_keyboard = keyboard_table_2;
	m_filterType = FILTER2;
	break;
    case HANGUL3F:
	m_keyboard = keyboard_table_3final;
	m_filterType = FILTER3;
	break;
    case HANGUL390:
	m_keyboard = keyboard_table_390;
	m_filterType = FILTER3;
	break;
    case HANGUL32:
	m_keyboard = keyboard_table_32;
	m_filterType = FILTER3;
	break;
    default:
	m_keyboard = keyboard_table_2;
	m_filterType = FILTER2;
    }
}

ComposerBase::~ComposerBase()
{
}

void ComposerBase::syllabicate()
{
    widestring text = getPreeditString();
    m_buf.clear();
    /* do not clear preedit string before commit
    widestring null;
    preeditUpdate(null);
     */
    commit(text);
}

bool ComposerBase::complete(wchar_t ch)
{
    widestring text = getPreeditString();
    m_buf.clear();

    /* do not clear preedit string before commit
    widestring null;
    preeditUpdate(null);
    */
    commit(text);

    if (ch != 0) {
	text = ch;
	commit(text);
	return true;
    }

    return false;
}

bool ComposerBase::backspace()
{
    if (m_buf.backspace()) {
	widestring text = getPreeditString();
	preeditUpdate(text);
	return true;
    }
    return false;
}

void ComposerBase::reset()
{
    if (m_buf.size() != 0) {
	widestring text = getPreeditString();
	m_buf.clear();
	commit(text);
    }
}

wchar_t ComposerBase::translateToJamo(int ascii)
{
    wchar_t ch = 0;

    if (ascii >= '!'  && ascii <= '~') {
	ch = m_keyboard[ascii - '!'];
    } else if (ascii == ' ') {
	ch = ascii;
    }

    return ch;
}

bool ComposerBase::filter(int ascii)
{
    wchar_t ch = translateToJamo(ascii);
    if (m_filterType == FILTER2) 
	return filter2(ch);
    else 
	return filter3(ch);
}

bool ComposerBase::filter2(wchar_t ch)
{
    wchar_t jong_ch;
    wchar_t comp_ch;
    widestring text;

    if (m_buf.hasJongseong()) {
	if (hangul_is_choseong(ch)) {
	    jong_ch = hangul_choseong_to_jongseong(ch);
	    comp_ch = hangul_compose(m_buf.m_jongseong, jong_ch);
	    if (hangul_is_jongseong(comp_ch)) {
		m_buf.push(comp_ch);
	    } else {
		syllabicate();
		m_buf.push(ch);
		goto update;
	    }
	    goto update;
	}
	if (hangul_is_jungseong(ch)) {
	    wchar_t pop, peek;
	    pop = m_buf.pop();
	    peek = m_buf.peek();

	    if (hangul_is_jungseong(peek)) {
		m_buf.m_jongseong = 0;
		syllabicate();
		m_buf.push(hangul_jongseong_to_choseong(pop));
		m_buf.push(ch);
	    } else {
		wchar_t choseong = 0, jongseong = 0; 
		hangul_jongseong_dicompose(m_buf.m_jongseong,
					   &jongseong, &choseong);
		m_buf.push(jongseong);
		syllabicate();
		m_buf.push(choseong);
		m_buf.push(ch);
	    }
	    goto update;
	}
    } else if (m_buf.hasJungseong()) {
	if (hangul_is_choseong(ch)) {
	    if (m_buf.hasChoseong()) {
		jong_ch = hangul_choseong_to_jongseong(ch);
		if (hangul_is_jongseong(jong_ch)) {
		    m_buf.push(jong_ch);
		} else {
		    syllabicate();
		    m_buf.push(ch);
		    goto update;
		}
	    } else {
		m_buf.push(ch);
	    }
	    goto update;
	}
	if (hangul_is_jungseong(ch)) {
	    comp_ch = hangul_compose(m_buf.m_jungseong, ch);
	    if (hangul_is_jungseong(comp_ch)) {
		m_buf.push(comp_ch);
	    } else {
		syllabicate();
		m_buf.push(ch);
		goto update;
	    }
	    goto update;
	}
    } else if (m_buf.hasChoseong()) {
	if (hangul_is_choseong(ch)) {
	    comp_ch = hangul_compose(m_buf.m_choseong, ch);
	    if (hangul_is_choseong(comp_ch)) {
		m_buf.push(comp_ch);
	    } else {
		syllabicate();
		m_buf.push(ch);
		goto update;
	    }
	    goto update;
	}
	if (hangul_is_jungseong(ch)) {
	    m_buf.push(ch);
	    goto update;
	}
    } else {
	if (hangul_is_choseong(ch)) {
	    m_buf.push(ch);
	    goto update;
	}
	if (hangul_is_jungseong(ch)) {
	    m_buf.push(ch);
	    goto update;
	}
    }

    return complete(ch);

update:
    text = getPreeditString();
    preeditUpdate(text);
    return true;
}

bool ComposerBase::filter3(wchar_t ch)
{
    widestring text;

    if (hangul_is_choseong(ch)) {
	if (!m_buf.hasChoseong()) {
	    m_buf.push(ch);
	    goto update;
	}
	if (hangul_is_choseong(m_buf.peek())) {
	    wchar_t choseong =
		hangul_compose(m_buf.m_choseong, ch);
	    if (choseong) {
		m_buf.push(choseong);
		goto update;
	    }
	}
	syllabicate();
	m_buf.push(ch);
	goto update;
    }

    /* jungseong */
    if (hangul_is_jungseong(ch)) {
	if (!m_buf.hasJungseong()) {
	    m_buf.push(ch);
	    goto update;
	}
	if (hangul_is_jungseong(m_buf.peek())) {
	    wchar_t jungseong =
		hangul_compose(m_buf.m_jungseong, ch);
	    if (jungseong) {
		m_buf.push(jungseong);
		goto update;
	    }
	}
	syllabicate();
	m_buf.push(ch);
	goto update;
    }

    /* jongseong */
    if (hangul_is_jongseong(ch)) {
	if (!m_buf.hasJongseong()) {
	    m_buf.push(ch);
	    goto update;
	}
	if (hangul_is_jongseong(m_buf.peek())) {
	    wchar_t jongseong =
		hangul_compose(m_buf.m_jongseong, ch);
	    if (jongseong) {
		m_buf.push(jongseong);
		goto update;
	    }
	}
	syllabicate();
	m_buf.push(ch);
	goto update;
    }

    return complete(ch);

update:
    text = getPreeditString();
    preeditUpdate(text);
    return true;
}
