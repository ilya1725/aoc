// pieces.cpp : Defines the entry point for the console application.
//
// The project just explores the reference member variables: store the class variable as
// a reference and update the object from th eoutside.
//

#include <vector>
#include <iostream>

using namespace std;

class PieceItem {
	private:
		int m_a;
		int m_b;
	public:
		PieceItem(int a, int b) :
			m_a(a), m_b(b) {}

		friend ostream& operator<< (ostream& stream, const PieceItem& p) {
			stream << "[";
			stream << p.m_a << "," << p.m_b;
			stream << "]";
			return stream;
		}
};

class Peer {
	private:
	vector<PieceItem>& m_pieces;
	string m_ip;
	int m_port;
	string m_infoHash;

	public:
		Peer(string ip, int port, string infoHash, vector<PieceItem>& pieces) :
			m_ip(ip), m_port(port), m_infoHash(infoHash), m_pieces(pieces) {}

		friend ostream& operator<< (ostream& stream, const Peer& peer) {
			stream << "[" << peer.m_ip.c_str() << "," << peer.m_port << "," << peer.m_infoHash.c_str() << "]";
			stream << "{";
			for (auto d : peer.m_pieces) {
				stream << d;
			}
			stream << "}";
			return stream;
		}
};

int main(void) {
	
	vector<PieceItem> pieces = { {1, 2}, {13, 14} };

	Peer peer("IP", 1234, "Hash", pieces);

	pieces.push_back({34,35});

	cout << peer << endl;

	return 0;
}