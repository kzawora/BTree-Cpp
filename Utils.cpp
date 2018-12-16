// funkcje pomocnicze, np wizualizacja drzewa
class bytearray {
public:
	char* arr;
	int size;
	bytearray(int size) {
		arr = new char[size];
	}
	~bytearray() {
		delete[] arr;
	}
};