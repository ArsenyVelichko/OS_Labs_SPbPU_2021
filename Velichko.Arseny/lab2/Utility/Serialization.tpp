template<class T, class>
void Serialization::serialize(char* buf, const T& value) {
	auto intBuf = reinterpret_cast<T*>(buf);
	*intBuf = value;
}

template<class T, class>
T Serialization::parse(const char* buf) {
	auto intBuf = reinterpret_cast<const T*>(buf);
	return *intBuf;
}
