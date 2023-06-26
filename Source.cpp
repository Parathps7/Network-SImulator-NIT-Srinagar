#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include <cstdlib>
#include <unistd.h>
#include<stdio.h>
#include <cstdlib>


#define ll long long int
using namespace std;

queue<int> tokenqueue;
vector<string> allnetworks;

int rnos = 0;
void remove(std::vector<string>& v)
{
	auto end = v.end();
	for (auto it = v.begin(); it != end; ++it) {
		end = std::remove(it + 1, end, *it);
	}

	v.erase(end, v.end());
}	
											//removing duplicates from vector
class enddevice										//class of enddevice
{
public:
	string macaddress;
	string data;
	string ipv4;
	string ipv6;
	string subnet;
	enddevice()									//constructor for assigning random mac address
	{
		subnet = "255.255.255.0";
		
		int a = 0;
		for (int i = 0; i <= 16; i++)
		{
			if (a == 2)
			{
				macaddress.push_back('-');
				a = 0;
			}

			else
			{
				int random = 0 + (rand() % 15);
				char ch;
				if (random >= 10)
				{
					random = random - 10;
					random += 65;
					ch = (char)random;
				}
				else
				{
					random += 48;
					ch = (char)random;
				}
				macaddress.push_back(ch);
				a++;
			}
		}
	}
};
class hub
{
public:
	string data;
};
class switchdevice
{
public:
	string data;
	vector<string> connecteddevices;
	string gateway;
	void macadd(enddevice obj)
	{
		connecteddevices.push_back(obj.macaddress);
	}
};
class bridge
{
public:
	string data;
};
class router
{
public:
	vector<string> gateaddress;
	vector<string> network;
	vector<int> connectedrouter;
	int id;
	string name;

	//the array index is the Network Destination
	//the data held inside that index is the number or hops required.
	int* dest_network;
	string* next_hop;
	int* num_of_hops;

	void printRoutingTable()
	{
		cout << this->name << " Routing Table:" << endl;
		cout << "  Destination      Next Hop    Number of Hops" << endl;
		for (int i = 1; i < rnos + 2; i++) {
			cout << setw(8) << "N" << i << " " << this->dest_network[i] << "  " << setw(10) << this->next_hop[i] << "  " << setw(10) << setw(10) << this->num_of_hops[i] << endl;
		}
	}
	void addNetwork(int x)
	{
		this->dest_network[x] = 0;
		this->next_hop[x] = "* (direct)";
		this->num_of_hops[x] = 0;
	}
	router()
	{
		this->name = "No named";
		this->dest_network = new int[rnos + 2];
		this->next_hop = new string[rnos + 2];
		this->num_of_hops = new int[rnos + 2];

		for (int i = 0; i < rnos + 2; i++) {
			this->dest_network[i] = -1;
		}
	}
	void gateadd(switchdevice obj)
	{
		gateaddress.push_back(obj.gateway);
	}
	void connectedrouters(int i)
	{
		int n;
		cout << "How many router are connected to router " << i + 1<<" : ";
		cin >> n;
		for (int j = 1; j <= n; j++)
		{
			int k;
			cout << "Enter the id : ";
			cin >> k;
			if (i + 1 < k && abs(i + 1 - k) > 1)
			{
				string temp;
				temp += "10";
				char ch = (char)k + 48;
				temp.push_back(ch);
				temp += ".0.0.0";
				network.push_back(temp);
				allnetworks.push_back(temp);
			}
			else if (i + 1 < k)
			{
				string temp;
				temp += "10";
				char ch = (char)i + 1 + 48;
				temp.push_back(ch);
				temp += ".0.0.0";
				network.push_back(temp);
				allnetworks.push_back(temp);
			}
			else
			{
				if (i + 1 - k > 1)
				{
					string temp;
					temp += "10";
					char ch = (char)(i + 1 + 48);
					temp.push_back(ch);
					temp += ".0.0.0";
					network.push_back(temp);
					allnetworks.push_back(temp);
				}
				else
				{
					string temp;
					temp += "10";
					char ch = (char)k + 48;
					temp.push_back(ch);
					temp += ".0.0.0";
					network.push_back(temp);
					allnetworks.push_back(temp);
				}
			}
			connectedrouter.push_back(k);
		}
	}
	
};


void changeNameTo(router& r, string newName) {
	r.name = newName;
}

string intToStr(int x) {

	stringstream convert_to_string;

	convert_to_string << x;
	return convert_to_string.str();
}

char exor(char a, char b) // perform exor operation
{
	if (a == b)
		return '0';
	else
		return '1';
}
void modulo2div(string& data, string key, int datalen, int keylen)
{
	char temp[20], rem[20];
	for (int i = 0; i < keylen; i++)
		rem[i] = data[i]; // considering keylen-1 bits of data for each step of binary division/EXOR

	for (int j = keylen; j <= datalen; j++)
	{
		for (int i = 0; i < keylen; i++)
			temp[i] = rem[i]; // remainder of previous step is divident of current step

		if (rem[0] == '0') // if 1's bit of remainder is 0 then shift the rem by 1 bit
		{
			for (int i = 0; i < keylen - 1; i++)
				rem[i] = temp[i + 1];
		}
		else // else exor the divident with generator polynomial
		{
			for (int i = 0; i < keylen - 1; i++)
				rem[i] = exor(temp[i + 1], key[i + 1]);
		}
		rem[keylen - 1] = data[j]; // appending next bit of data to remainder obtain by division
	}
	data.erase(8);
	for (int i = 0; i < 8; i++)
	{
		data.push_back(rem[i]);
	}
	cout << "CRC : " << rem << "\nDataword : " << data; // remainder obtain is crc
}
int checkdiv(string& data, string key, int datalen, int keylen)
{
	char temp[20], rem[20];
	for (int i = 0; i < keylen; i++)
		rem[i] = data[i]; // considering keylen-1 bits of data for each step of binary division/EXOR

	for (int j = keylen; j <= datalen; j++)
	{
		for (int i = 0; i < keylen; i++)
			temp[i] = rem[i]; // remainder of previous step is divident of current step

		if (rem[0] == '0') // if 1's bit of remainder is 0 then shift the rem by 1 bit
		{
			for (int i = 0; i < keylen - 1; i++)
				rem[i] = temp[i + 1];
		}
		else // else exor the divident with generator polynomial
		{
			for (int i = 0; i < keylen - 1; i++)
				rem[i] = exor(temp[i + 1], key[i + 1]);
		}
		rem[keylen - 1] = data[j]; // appending next bit of data to remainder obtain by division
	}
	for (int i = 0; i < 8; i++)
	{
		if (rem[i] == '1')
		{
			cout << endl
				<< "Discard";
			return 0;
		}
	}
	data.erase(8);
	cout << endl
		<< "Extracted Data : " << data;
	return 0;
}
void err(string& data)
{
	int ra = (rand() % (32 - 0 + 1)) + 0; // will generate random number b/w 0 - 32
	if (ra < data.size())
	{
		if (data[ra] == '0')
			data[ra] = '1';
		else
			data[ra] = '0';
	}
}
void Ripv4(router& from, router& to) 
{
	for (int i = 1; i < rnos + 2; i++) 
	{
		if (from.dest_network[i] != -1) 
		{
			if ((to.dest_network[i] > from.dest_network[i]) || (to.dest_network[i] == -1)) 
			{
				to.dest_network[i] = from.dest_network[i] + 1;
				to.next_hop[i] = from.name;
				to.num_of_hops[i] = from.dest_network[i] + 1;

			}
		}
	}
}
void stopandwait(int frames[], int n)
{
	int i = 1, tt = 0;
	while (i <= n) {
		int z = 0;
		for (int k = i; k <= n; k++)
		{
			cout << "Sending Frame " << k << "..." << endl;
			int f = rand() % 3;
			if (f == 0)
			{
				cout << "Acknowledgment for Frame " << k << "..." << endl;
				z++;
			}
			else if (f == 1)
			{
				cout << "Delayed ack ";
				cout << endl << "Retransmitting Frame : " << k;
			}
			else {
				cout << "Timeout!! Frame Number : " << k << " Not Received" << endl;
				cout << endl << "Retransmitting Frame : " << k;
				break;
			}
		}
		cout << "\n";
		i = i + z;
	}
}
void transmission(ll& i, ll& N, ll& tf, ll& tt) {
	while (i <= tf) {
		int z = 0;
		for (int k = i; k < i + N && k <= tf; k++) {
			cout << "Sending Frame " << k << "..." << endl;
			tt++;
		}
		for (int k = i; k < i + N && k <= tf; k++) {
			int f = rand() % 2;
			if (!f) {
				cout << "Acknowledgment Recieved for Frame " << k << "..." << endl;
				z++;
			}
			else {
				cout << "Timeout  Frame Number : " << k << " Not Received" << endl;
				cout << "Retransmitting Window..." << endl;
				break;
			}
		}
		cout << "\n";
		i = i + z;
	}
}
void tokenpassing(enddevice device[], queue<int> tokenqueue, int station)
{
	queue<int>g = tokenqueue;
	int rep;
	while (!g.empty()) {
		cout << g.front() << " Station has the token and access to send data now." << endl;
		cout << endl << "3Enter data : ";
		cin >> device[g.front() - 1].data;
		rep = g.front();
		if (device[g.front() - 1].data == "0")
			break;
		else
		{
			cout << "DATA SENT" << endl;
			g.pop();
			g.push(rep);
		}
	}
}

std::string GetCommandOutput(const std::string& command) {
    std::string result;
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    constexpr int kBufferSize = 128;
    char buffer[kBufferSize];
    while (fgets(buffer, kBufferSize, pipe)) {
        result += buffer;
    }

    _pclose(pipe);
    return result;
}

int main()
{
	int a;
	cout << "What type of connection do you want : ";
	cout << endl << "1.Dedicated connection ";
	// cout << endl << "2.Star Topology ";
	// cout << endl << "3.Bus Topology ";
	cout << endl << "2.Token passing protocol ";
	cout << endl << "3.Flow control protocol ";
	cout << endl << "4.Switch ";
	cout << endl << "5.Router ";
	cout << endl << "6.DNS ";
	cout << endl << "7.HTTP ";
	cout << endl;
	cin >> a;
	if (a == 1)			//Dedicated Connection
	{
		int d;
		cout << "Enter No. of devices : ";
		cin >> d;
		enddevice dedicated[d];                         //creatation of enddevice
		int dt = 0;
		while (dt < d)
		{
			cout << "  PC - " << dt;
			dt++;
		}
		dt = 0;
		int ds, dr;
		string da;
		cout << endl << "Enter the device from where you want to send data : ";
		cin >> ds;
		cout << "Enter the device where you want to recieve data : ";
		cin >> dr;
		cout << "Enter Data : ";
		cin >> da;
		dedicated[ds].data = da;
		cout << "Devices state before sending the Data : " << endl;
		while (dt < d)
		{
			cout << endl << "  PC - " << dt << " Data - " << dedicated[dt].data;
			dt++;
		}
		dt = 0;
		dedicated[dr].data = dedicated[ds].data;
		dedicated[ds].data = "\0";
		cout << endl << "Devices state After sending the Data : " << endl;
		while (dt < d)
		{
			cout << endl << "  PC - " << dt << " Data - " << dedicated[dt].data;
			dt++;
		}
	}
	
	else if (a == 2)
	{
		enddevice device[10];
		cout << "Enter the number of stations : " << endl;
		int station, sendingstation;
		cin >> station;
		cout << "Enter the number stations which want to send data:" << endl;
		cin >> sendingstation;
		cout << "Enter station queue which are likely to send data:" << endl;
		int temp;
		for (int l = 0; l < sendingstation; l++)
		{
			cin >> temp;
			tokenqueue.push(temp);
		}
		tokenpassing(device, tokenqueue, station);
	}
	else if (a == 3)
	{
		int deviceno;
		cout << "Enter the number of device you want to connect : ";
		cin >> deviceno;
		enddevice switchdevice[deviceno];
		int dt = 0;
		while (dt < deviceno)
		{
			cout << "  PC - " << dt;
			dt++;
		}
		dt = 0;
		int n;
		cout << endl << "Enter no of data size : ";
		cin >> n;
		int da[n];
		int ds, dr;
		cout << endl << "Enter the device from where you want to send data : ";
		cin >> ds;
		cout << " MAC address of device :" << switchdevice[ds].macaddress;
		cout << endl << "Enter the device where you want to recieve data : ";
		cin >> dr;
		cout << " MAC address of device :" << switchdevice[dr].macaddress;
		cout << endl << "Enter Data : ";
		for (int i = 0; i < n; i++)
			cin >> da[i];

		cout << endl << "Enter your choice for flow control protocol : ";
		cout << endl << "1.Stop And Wait Arq";
		cout << endl << "2.Go Back-N Arq" << endl;
		int cho;
		cin >> cho;
		if (cho == 1)
			stopandwait(da, n);
		else if (cho == 2)
		{
			cout << "              GO BACK-N ARQ";
			ll tf, N, tt = 0;
			tf = n;
			cout << endl << "Enter the Window Size : ";
			cin >> N;
			ll i = 1;
			transmission(i, N, tf, tt);
			cout << "Total number of frames which were sent and resent are : " << tt << endl;
		}
	}
	else if (a == 4)
	{
		int deviceno;
		cout << "Enter the number of device you want to connect : ";
		cin >> deviceno;
		enddevice switchdevice[deviceno];
		int dt = 0;
		while (dt < deviceno)
		{
			cout << "  PC - " << dt;
			dt++;
		}
		dt = 0;
		int ds, dr;
		string da;
		cout << endl << "Enter the device from where you want to send data : ";
		cin >> ds;
		cout << " MAC address of device :" << switchdevice[ds].macaddress;
		cout << endl << "Enter the device where you want to recieve data : ";
		cin >> dr;
		cout << " MAC address of device :" << switchdevice[dr].macaddress;
		cout << endl << "Enter Data : ";
		cin >> da;
		switchdevice[ds].data = da;
		vector<string> learning;												//address learning
		for (int i = 0; i < deviceno; i++)
		{
			learning.push_back(switchdevice[i].macaddress);
		}
		cout << "Firstly address learning happens" << endl;
		while (dt < deviceno)
		{
			cout << endl << "  PC - " << dt << "  Address learned " ;
			dt++;
		}
		dt = 0;
		switchdevice[dr].data = switchdevice[ds].data;
		switchdevice[ds].data = "\0";
		cout << endl << "Devices state After sending the Data : " << endl;
		while (dt < deviceno)
		{
			if (dt == dr)
				cout << endl << "DATA RECIEVED at PC - " << dt;
			else
				cout << endl << "Wrong device" << " PC - " << dt;
			dt++;
		}
	}
	else if (a == 5)															//Creating and configuring a router
	{
		int rno;
		cout << endl << "How many routers do you want :";
		cin >> rno;
		rnos = rno;
		router r[rno];
		for (int i = 0; i < rno; i++)
		{
			r[i].connectedrouters(i);											//connection between routers
		}
		int sno[rno];
		int sums = 0;															//sum of switches
		cout << endl;
		for (int i = 0; i < rno; i++)
		{
			cout  << "Enter number of Switches in Router - " << i+1 <<" : ";	
			cin >> sno[i];
			sums += sno[i];
		}
		switchdevice s[sums];
		int device[sums];
		int sume = 0;															//sum of enddevices
		cout << endl;
		for (int i = 0; i < sums; i++)
		{
			cout  << "Enter number of Enddevices in Switch - " << i+1 <<" : ";
			cin >> device[i];
			sume += device[i];
		}
		enddevice pc[sume];
		int num = 0;
		int ran = 0;
		for (int k = 0; k < rno; k++)																		//routers
		{
			string net;
			char c = (char)(k + 1 + 48);
			net.push_back(c);
			net += "0.0.0.0";
			r[k].network.push_back(net);
			allnetworks.push_back(net);
			for (int i = 0; i < sno[k]; i++)																//switches
			{
				int j;
				for (j = 0; j < device[i]; j++)																//enddevices
				{
					char ch = (char)(k + 1 + 48);
					pc[num].ipv4.push_back(ch);
					pc[num].ipv4.push_back('0');
					pc[num].ipv4.push_back('.');
					char ch2 = (char)(i + 1 + 48);
					pc[num].ipv4.push_back(ch2);
					pc[num].ipv4 += ".0.";
					char c = (char)j + 48 + 1;
					pc[num].ipv4.push_back(c);
					s[ran].macadd(pc[num]);
					num++;
				}
				char ch = (char)(k + 1 + 48);
				s[ran].gateway.push_back(ch);
				s[ran].gateway.push_back('0');
				s[ran].gateway.push_back('.');
				char ch2 = (char)(i + 1 + 48);
				s[ran].gateway.push_back(ch2);
				s[ran].gateway += ".0.";
				char c = (char)48;
				s[ran].gateway.push_back(c);
				r[k].gateadd(s[ran]);
				ran++;
			}
		}
		int choice;
		cout << endl << "ARP" ;
		cout << endl << "1.Arp table" ;
		cout << endl << "2.Find mac of host using arp";
		cout << endl << "Enter your choice : ";
		cin >> choice;
		if (choice == 1)
		{
			cout << endl << "IPv4            " <<"Physical Address   " << endl;
			for (int i = 0; i < sume; i++)
			{
				cout << endl << pc[i].ipv4  << "	     " << pc[i].macaddress;
			}
		}
		else if (choice == 2)
		{
			string arpreq;
			cout << endl << "Enter the ipv4 address of the host : ";
			cin >> arpreq;
			int f = 0;
			for (int i = 0; i < sume; i++)
			{
				if (pc[i].ipv4 == arpreq)
				{
					cout << endl << "MAC - " << pc[i].macaddress;
					f = 1;
					break;
				}
			}
			if (f == 0)
				cout << endl << "Invalid ipv4.";
		}
		else
		{
			cout << endl << "Wrong choice" << endl;
		}
	
		int rc;																			//routing choice
		remove(allnetworks);
		cout << endl;
		cout << endl << "What type of routing do you want :" << endl << "1.Static Routing " << endl << "2.Dynamic Routing(Ripv4 Protocol)" << endl;
		cin >> rc;
		if (rc == 1)
		{
			for (int i = 0; i < rno; i++)
			{
				cout << endl <<endl << "Routing Table : " << i + 1;
				cout << endl << endl << "Destination                 HOP       " ;
				for (auto j : allnetworks)
				{
					cout << endl << j;
					if (j[3] == '.')
					{
						cout << "                       " << abs(i + 1 - (int)j[2] + 48) ;
					}
					if (j[2] == '.')
					{
						cout << "                        " << abs(i + 1 - (int)j[0] + 48) ;
					}
				}
			}
		}
		else if (rc == 2)
		{
			router* arrayofRouters;

			arrayofRouters = new router[rnos + 1];
			for (int i = 1; i <= rno; i++) {
				arrayofRouters[i].name = "Router#" + intToStr(i);
				arrayofRouters[i].addNetwork(i);
				arrayofRouters[i].addNetwork(i + 1);

			}
			// Performing Ripv4 on routers and neighbough routers.
			for (int y = 1; y < rno; y++) 
			{
				//Ripv4 r1
				for (int i = 1; i <= rno; i++)
				{
					if (i == 1)
						Ripv4(arrayofRouters[i], arrayofRouters[i + 1]);
					else if (i == rno)
						Ripv4(arrayofRouters[i], arrayofRouters[i - 1]);
					else
					{
						Ripv4(arrayofRouters[i], arrayofRouters[i - 1]);
						Ripv4(arrayofRouters[i], arrayofRouters[i + 1]);
					}
				}
			}
			//output final table
			cout << endl;
			cout << "					FINAL TABLES					" << endl;
			for (int j = 1; j <= rno; j++)
			{
				arrayofRouters[j].printRoutingTable();
			}
		}
		else
		{
			cout << endl << "Wrong choice" << endl;
		}
	}
	else if(a==6)
	{
		string ip;
		cout<<"Enter Domain whose IP to get:";
		cin>>ip;
		std::string command = "nslookup "+ip; // Replace with your desired command
    	std::string output = GetCommandOutput(command);
    	std::cout << output;
		
	}
	else if(a==7)
	{
	
		std::system("python hello.py ");
	}
	else
		cout << "Invalid choice.";
	return 0;
}