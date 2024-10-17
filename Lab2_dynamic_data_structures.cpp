//������� 4
//��������� ���������, ������� �������� ������� ���������� � ������� ��
//����������.
//������ ������ �������� :
//� ����� ����������;
//� ����� �����;
//� ������� � �������� ���������;
//� �������� ���� ������.
//��������� ������ ������������ :
//� �������� ���� ������ � ���� ��������� ������;
//� ���������� � �������� ������;
//� �� ��������� ������ ����� � ���� ������ ����� ������ � �� �����������
//���������;
//� ����� ���� ������

#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

struct TicketRequest {
    string destination;
    string flight_num;
    string pass_name;
    string date;
    TicketRequest() : destination(""), flight_num(""), pass_name(""), date(""){}
    TicketRequest(const string& dest, const string& num, const string& name, const string& date) : destination(dest), flight_num(num), pass_name(name), date(date) {}
};
 
struct NodeT {
    TicketRequest data;
    NodeT* left;
    NodeT* right;
    NodeT(const TicketRequest& data) : data(data), left(nullptr), right(nullptr) {}
};

struct BinaryTree {
    NodeT* root;
    BinaryTree() : root(nullptr) {}
    BinaryTree(const TicketRequest& ticket)
    {
        NodeT* node = new NodeT(ticket);
        root = node;
    }
    ~BinaryTree()
    {
        if (root != nullptr) {
            Destroy(root);
            cout << "������ �������\n";
        }
        else cout << "������ ������(����������)\n";
    }

    void Destroy(NodeT* node)
    {
        if (node != nullptr) {
            Destroy(node->left);
            Destroy(node->right);
            delete node;
        }
    }
    NodeT* insert(NodeT* node, const TicketRequest& ticket) {
        if (node == nullptr) {
            return new NodeT(ticket);
        }
        else if (compareTickets(ticket, node->data)) {
            node->left = insert(node->left, ticket);
        }
        else if (compareTickets(node->data, ticket)) {
            node->right = insert(node->right, ticket);
        }
        else {
            cout << "������ ��� ��������� " + ticket.flight_num + " " + ticket.date + " " + ticket.pass_name <<endl;
        }
        return node;
    }

    void insert(const TicketRequest& ticket) {
        root = insert(root, ticket);
    }

    bool compareTickets(const TicketRequest& t1, const TicketRequest& t2) {
        if (t1.flight_num == t2.flight_num) {
            if (t1.date == t2.date) {
                return t1.pass_name < t2.pass_name; 
            }
            return t1.date < t2.date; 
        }
        return t1.flight_num < t2.flight_num;
    }

    bool compareTicketsWithoutPassenger(const TicketRequest& a, const TicketRequest& b) {
        if (a.flight_num == b.flight_num) {
            return a.date < b.date;
        }
        return a.flight_num < b.flight_num;
    }

    void inOrderTraversal(NodeT* node, ostream& outstream) {
        if (node) {
            inOrderTraversal(node->left, outstream);
            outstream << node->data.destination << " " << node->data.flight_num << " " << node->data.pass_name << " " << node->data.date << "\n";
            inOrderTraversal(node->right, outstream);
        }
    }
    int displayTickets() {
        if (root == nullptr) {
            cout << "������ ������" << endl;
            return 1;
        }
        else {
            inOrderTraversal(root, cout); 
            cout << endl;
            return 0;
        }
        
    }
    int displayTickets(const string& outputfile) {
        if (root == nullptr) {
            cout << "������ ������" << endl;
            return 1;
        }
        ofstream out(outputfile); //������� ���� ��� ����������, ������� ����  ���� ��� ���
        if (!out.is_open()){
            cout << "�� ������� ������� ���� " + outputfile + "\n";
            return 1;
        }
        inOrderTraversal(root, out); 
        out.close();
        return 0;
    }

    void removeTicket(const string& flightNumber, const string& date, const string& passengerName) {
        if (root == nullptr) {
            cout << "������ ������" << endl;
            return;
        }
        if (remove(root, TicketRequest("", flightNumber, passengerName, date))) {
            cout << "������ �������\n";
        }
        else {
            cout << "������ �� �������\n";
        }
    }

    void removeTicket(const string& flightNumber, const string& date) {
        int flag = 0;
        while (removeall(root, TicketRequest("", flightNumber, "", date))) {
            cout << "�������\n";
            flag = 1;
        }
        if (!flag) cout << "������ �� �������" << endl;
    }

    bool remove(NodeT*& node, const TicketRequest& fdata) {
        if (!node) return false;
        if (compareTickets(fdata, node->data)) {
            return remove(node->left, fdata); 
        }
        else if (compareTickets(node->data,fdata)) {
            return remove(node->right, fdata); 
        }        
        else {
            //cout << "������ " + node->data.flight_num + ", " + node->data.date + ", " + node->data.pass_name + "\n";
            deleteNode(node);
            return true;
        }
    }
    
    bool removeall(NodeT*& node, const TicketRequest& fdata) {
        if (!node) return false;
        
        if (compareTicketsWithoutPassenger(fdata, node->data)) {
            return removeall(node->left, fdata); 
        }
        else if (compareTicketsWithoutPassenger(node->data, fdata)) {
            return removeall(node->right, fdata); 
        }
        else {
            cout << "������ " + node->data.flight_num + ", " + node->data.date + ", " + node->data.pass_name + "\n";
            deleteNode(node);
            return true; 
        }
    }
    void deleteNode(NodeT*& node) {
        if (node->left == nullptr && node->right == nullptr) {
            delete node; 
            node = nullptr; // �������� ���������
        }
        // ���� � ����� ��������
        else if (!node->left) {
            NodeT* temp = node;
            node = node->right; // �������� �� ������� �������
            delete temp; 
        }
        else if (!node->right) {
            NodeT* temp = node;
            node = node->left;
            delete temp; 
        }
        // ���� � ����� ������
        else {
            NodeT*& minNode = FindMin(node->right); // ������� ��� ������� �� ������� ��������� 
            node->data = minNode->data; // �������� ������ ����� ��������
            remove(node->right, minNode->data); // � ���������� ������� ���� ����������� ������� �� ������� ���������
        }
    }
    NodeT*& FindMin(NodeT*& node) {
        return (node->left == nullptr) ? node : FindMin(node->left);

    }
    
    int SaveOptiones() {
        cout << "������� 1 - �� �������, 2 - � ����" << endl;
        int select, flag = 0;
        cin >> select;
        switch (select)
        {
            case 1: displayTickets();break;
            case 2: {cout << "������� ��� �����\n";
                string file;
                cin >> file;
                flag = displayTickets(file);
                if (flag != 0) return 1;
                else return 0;
                break;}
            default: return 2;
        }
        
    }

    int readFromFile(const string& filename) {
        ifstream infile;
        infile.open(filename);
        if (!infile.is_open()) {
            cout << "�� ������� ������� ���� " << filename << "\n";
            return 1;
        }

        string str;
        string destination, flightNumber, passengerName, date;
        stringstream obj;
        int count = 0;
        while (getline(infile, str))
        {
            obj << str + "\n";
            count++;
        }

        infile.close();
        for (int i = 0; i < count; i++)
        {
            string str;
            getline(obj, str);

            int index = str.find(',');
            destination = str.substr(0, index);
            str = str.substr(index + 1);

            index = str.find(',');
            flightNumber = str.substr(0, index);
            str = str.substr(index + 1);

            index = str.find(',');
            passengerName = str.substr(0, index);
            str = str.substr(index + 1);

            index = str.find(',');
            date = str.substr(0, index);
            str = str.substr(index + 1);
            insert(TicketRequest(destination, flightNumber, passengerName, date));

        }
        return 0;       
    }
};


int main()
{
    setlocale(LC_ALL, "rus");
    BinaryTree tree;

    if (tree.readFromFile("1.txt")) return 0;

   /* tree.insert(TicketRequest("������", "AA103", "������ �.�.", "2024-10-15"));
    tree.insert(TicketRequest("�����������", "AA101", "������ �.�.", "2024-10-16"));
    tree.insert(TicketRequest("����", "AA103", "������� �.�.", "2024-10-15")); 
    tree.insert(TicketRequest("������", "AA104", "������ �.�.", "2024-10-15")); 
    tree.insert(TicketRequest("������", "AA108", "������ �.�.", "2024-10-15"));
    tree.insert(TicketRequest("������", "AA103", "������ �.�.", "2024-10-15"));
    tree.insert(TicketRequest("������", "AA100", "������� �.�.", "2024-10-15"));
    tree.insert(TicketRequest("������", "AA103", "������ �.�.", "2023-10-15"));*/

    cout << "��� ������ � ������� �����������:\n";
    tree.displayTickets();

    //tree.removeTicket("AA103", "2024-10-15", "bhbhh �.�.");
    tree.removeTicket("AA104", "2024-10-15", "������ �.�.");
    tree.displayTickets();
    tree.removeTicket("AA103", "2024-10-15");

    int flag = tree.SaveOptiones();
    if (flag != 0) {
        cout << "�� ������� ����������� ������" << endl;
    }
    else cout << "������ �����������" << endl;

    return 0;
}

