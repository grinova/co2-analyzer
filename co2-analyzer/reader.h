#ifndef READER_H_
#define READER_H_


#define READER_BUFFER_SIZE	10;

class Reader {
public:
	Reader(const char * src);
	bool has_next();
	unsigned char next();
private:
	const char * buffer;
	unsigned char offset;
};

#endif /* READER_H_ */