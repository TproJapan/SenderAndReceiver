//TCP,UDP通信用ライブラリ
#pragma comment(lib, "ws2_32.lib")

//TCP,UDP通信用ヘッダ
//#include <sys/sock.h> //linux
#include <WinSock2.h> //windows
#include<cstdlib>
#include <iostream>

// inet_addr()関数で警告が出る場合は以下で警告を無効化する。
#pragma warning(disable:4996) 

// 受信関数
int recieve() {
	// ソケット通信winsockの立ち上げ
	// wsaDataはエラー取得等に使用する
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);   //MAKEWORD(2, 0)はwinsockのバージョン2.0ってこと

	// socket作成
	// socketは通信の出入り口 ここを通してデータのやり取りをする
	// socket(アドレスファミリ, ソケットタイプ, プロトコル)
	SOCKET sock;
	sock = socket(AF_INET, SOCK_DGRAM, 0);  //AF_INETはIPv4、SOCK_DGRAMはUDP通信、0は？

	// アドレス等格納
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;  //IPv4
	addr.sin_port = htons(12345);   //通信ポート番号設定
	addr.sin_addr.S_un.S_addr = INADDR_ANY; // INADDR_ANYはすべてのアドレスからのパケットを受信する

	// バインド
	// アドレス等の情報をsocketに登録する
	bind(sock, (struct sockaddr*) & addr, sizeof(addr));

	// ブロッキング、ノンブロッキングの設定
	//（任意、必須コードではない（ソケットの初期設定はブロッキングモードなため）
	// val = 0 : ブロッキングモード データが受信されるまで待機
	// val = 1 : ノンブロッキング データが受信されなくても次の処理へ
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);

	// バッファ ここに受信したデータが入る サイズは自由に決められるが、char配列
	char buf[2048];
	memset(buf, 0, sizeof(buf)); // 複数回受信するときは前回値が残らないようにこのように バッファを0でクリアするのが一般的 メモリー操作関数

	// 受信
	// recv(ソケット, 受信するデータの格納先, データのバイト数, フラグ);
	// バインドしていない場合は recvfrom(sock, buf, 5, 0, (struct sockaddr *)&addr, sizeof(addr)); でもOK？
	recv(sock, buf, sizeof(buf), 0);

	// 出力
	printf("%s\n", buf);

	// socketの破棄
	closesocket(sock);

	// winsockの終了
	WSACleanup();

	return 0;
}

//送信関数
int send() {
	// ソケット通信winsockの立ち上げ
	// wsaDataはエラー取得等に使用する
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);   //MAKEWORD(2, 0)はwinsockのバージョン2.0ってこと

	// socket作成
	// socketは通信の出入り口 ここを通してデータのやり取りをする
	// socket(アドレスファミリ, ソケットタイプ, プロトコル)
	SOCKET sock;
	sock = socket(AF_INET, SOCK_DGRAM, 0);  //AF_INETはIPv4、SOCK_DGRAMはUDP通信、0は？

	// アドレス等格納
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;  //IPv4
	addr.sin_port = htons(12345);   //通信ポート番号設定
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // 送信アドレスを127.0.0.1に設定

	// バッファ ここに送信するデータを入れる  サイズは自由に決められるが、char配列
	char buf[2048] = "HELLO";

	// 送信
	// sendto(ソケット, 送信するデータ, データのバイト数, フラグ, アドレス情報, アドレス情報のサイズ);
	// 送信するデータに直接文字列 "HELLO" 等を入れることもできる
	// バインドしている場合は send(sock, buf, 5, 0); でもOK？
	sendto(sock, buf, sizeof(buf), 0, (struct sockaddr*) & addr, sizeof(addr));

	// socketの破棄
	closesocket(sock);

	// winsockの終了
	WSACleanup();

	return 0;
}

int main() {
	send();
	recieve();

	system("pause");
	return 0;
}