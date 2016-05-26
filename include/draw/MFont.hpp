/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MFONT_HPP_
#define MFONT_HPP_

namespace draw {

/*! \brief Monochrome Font */
class MFont {
public:

	static const char * charset();

	virtual int len(const char * str) const { return 0; }
	virtual int size() const { return 0; }

	enum {
		CHARSET_SIZE = 95
	};

protected:
	static int to_charset(char ascii);

};

};

#endif /* MFONT_HPP_ */
