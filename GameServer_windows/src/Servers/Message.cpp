#include"Client.h"
#include"Message.h"



int bytesToInt(byte* bytes, int index) {


	int addr = bytes[index] & 0xFF;

	addr |= ((bytes[index + 1] << 8) & 0xFF00);
	addr |= ((bytes[index + 2] << 16) & 0xFF0000);
	addr |= ((bytes[index + 3] << 24) & 0xFF000000);
	return addr;

}

void intToByte(int i, byte* a, int size = 4) {

	byte bytes[4];
	memset(bytes, 0, sizeof(byte) * size);
	bytes[0] = (byte)(0xff & i);
	bytes[1] = (byte)((0xff00 & i) >> 8);
	bytes[2] = (byte)((0xff0000 & i) >> 16);
	bytes[3] = (byte)((0xff000000 & i) >> 24);
	std::copy(std::begin(bytes), std::end(bytes), a);
}

std::string byteToString(byte *byte_arr, int index, int count)
{
	char *p = new char[count];
	std::string s;
	for (int i = 0; i < count; i++) {
		p[i] = (char)byte_arr[index+i];
		s.push_back(p[i]);
	}	
	return s;
}

byte* StringTobyte(std::string str) {
	byte* px = (byte*)str.c_str();
	return px;
}

int byteLength(byte* b) {
	int a = 0;
	while (*(b+a)!='\0')
	{
		a++;
	}
	return a;
}


void BytesConcat_int(byte *a, byte*b, byte*c,int length=8){


	for (int i = 0, j=0; i < length; i++)
	{
		if (i<4)
		{
			c[i] = a[i];
		}
		else
		{
			c[i] = b[j];
			j++;
		}
		
	}
}
void BytesConcat_string(byte *a, byte*b, byte*c,int length=8) {

	int i = 0;
	for (; i < length; i++)
	{
		c[i] = a[i];
	}
	int j = 0;
	while (*(b+j)!='\0')
	{
		c[i + j] = b[j];
		j++;
	}
}



int Message::StartIndex() {
	return this->startIndex;
}

int Message::RemainSize() {
	return Length_m - this->startIndex;
}
void Message::ReadMessage(int newDateAmount, Client* client) {
	int  startIndex = 0;
	startIndex += newDateAmount;

	while (true)
	{
		if (startIndex <= 4) {
			return;
		}
		int count= bytesToInt(date, 0);
		if ((startIndex - 4) >= count)
		{			
			Common::RequestCode requestCode = (Common::RequestCode)bytesToInt(date,4);

			Common::ActionCode actionCode = (Common::ActionCode)bytesToInt(date,8);
			
			std::string s = byteToString(date, 12, count - 8);

			client->OnProcessMessage(requestCode, actionCode, s);
			
			byte b[Length_m];
			std::copy(std::begin(date), std::end(date), std::begin(b));
			if (startIndex - 4 - count > 0) {
					for (int i = 0; i < startIndex - 4 - count; i++)
					{
						date[i] = b[count + 4+i];
					}
			}			
			startIndex -= (count + 4);
		}
		else
		{
			break;
		}
	}
}

 byte* Message::PackData(Common::ActionCode actionCode, std::string date) {

	byte requestCodeBytes[4] = { 0 };
	intToByte((int)actionCode, requestCodeBytes);

	byte* dataBytes = (byte*)date.c_str();

	int dataAmount = 4 + byteLength(dataBytes);

	byte dataAmountBytes[4] = { 0 };
	intToByte(dataAmount, dataAmountBytes);
	byte newBytes_first[8] = { 0 };

	BytesConcat_int(dataAmountBytes, requestCodeBytes, newBytes_first);

	int count = 8 + byteLength(dataBytes);

	byte *newBytes_Second = new byte[count];

	BytesConcat_string(newBytes_first, dataBytes, newBytes_Second);

	return newBytes_Second;
}





//int bytesToInt(byte* bytes, int size = 4)
//{
//	int a = bytes[0] & 0xFF;
//	a |= ((bytes[1] << 8) & 0xFF00);
//	a |= ((bytes[2] << 16) & 0xFF0000);
//	a |= ((bytes[3] << 24) & 0xFF000000);
//	return a;
//
//}