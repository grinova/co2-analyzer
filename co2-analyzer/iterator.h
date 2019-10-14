#ifndef ITERATOR_H_
#define ITERATOR_H_


class Iterator {
public:
	Iterator(unsigned int * buffer, unsigned char len, unsigned char offset);
	bool has_next();
	unsigned int next();

private:
	unsigned int * buffer;
	unsigned char len;
	unsigned char i;
	unsigned char offset;
};

#endif /* ITERATOR_H_ */
