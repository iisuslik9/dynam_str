//Вариант 4
//Составить программу, которая содержит текущую информацию о заявках на
//авиабилеты.
//Каждая заявка содержат :
//• пункт назначения;
//• номер рейса;
//• фамилию и инициалы пассажира;
//• желаемую дату вылета.
//Программа должна обеспечивать :
//• хранение всех заявок в виде двоичного дерева;
//• добавление и удаление заявок;
//• по заданному номеру рейса и дате вылета вывод заявок с их последующим
//удалением;
//• вывод всех заявок

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
            cout << "дерево удалено\n";
        }
        else cout << "дерево пустое(деструктор)\n";
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
            cout << "Заявка уже добавлена " + ticket.flight_num + " " + ticket.date + " " + ticket.pass_name <<endl;
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
            cout << "Дерево пустое" << endl;
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
            cout << "Дерево пустое" << endl;
            return 1;
        }
        ofstream out(outputfile); //откроет файл для перезаписи, создаст файл  если его нет
        if (!out.is_open()){
            cout << "Не удалось открыть файл " + outputfile + "\n";
            return 1;
        }
        inOrderTraversal(root, out); 
        out.close();
        return 0;
    }

    void removeTicket(const string& flightNumber, const string& date, const string& passengerName) {
        if (root == nullptr) {
            cout << "Дерево пустое" << endl;
            return;
        }
        if (remove(root, TicketRequest("", flightNumber, passengerName, date))) {
            cout << "заявка удалена\n";
        }
        else {
            cout << "заявка не найдена\n";
        }
    }

    void removeTicket(const string& flightNumber, const string& date) {
        int flag = 0;
        while (removeall(root, TicketRequest("", flightNumber, "", date))) {
            cout << "удалена\n";
            flag = 1;
        }
        if (!flag) cout << "заявки не найдены" << endl;
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
            //cout << "заявка " + node->data.flight_num + ", " + node->data.date + ", " + node->data.pass_name + "\n";
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
            cout << "заявка " + node->data.flight_num + ", " + node->data.date + ", " + node->data.pass_name + "\n";
            deleteNode(node);
            return true; 
        }
    }
    void deleteNode(NodeT*& node) {
        if (node->left == nullptr && node->right == nullptr) {
            delete node; 
            node = nullptr; // обнуляем указатель
        }
        // Узел с одним ребенком
        else if (!node->left) {
            NodeT* temp = node;
            node = node->right; // заменяем на правого ребенка
            delete temp; 
        }
        else if (!node->right) {
            NodeT* temp = node;
            node = node->left;
            delete temp; 
        }
        // Узел с двумя детьми
        else {
            NodeT*& minNode = FindMin(node->right); // находим мин элемент из правого поддерева 
            node->data = minNode->data; // Копируем данные этого элемента
            remove(node->right, minNode->data); // и рекурсивно удаляем этот минимальный элемент из правого поддерева
        }
    }
    NodeT*& FindMin(NodeT*& node) {
        return (node->left == nullptr) ? node : FindMin(node->left);

    }
    
    int SaveOptiones() {
        cout << "вывести 1 - на консоль, 2 - в файл" << endl;
        int select, flag = 0;
        cin >> select;
        switch (select)
        {
            case 1: displayTickets();break;
            case 2: {cout << "введите имя файла\n";
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
            cout << "Не удалось открыть файл " << filename << "\n";
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

   /* tree.insert(TicketRequest("Москва", "AA103", "Иванов И.И.", "2024-10-15"));
    tree.insert(TicketRequest("Новосибирск", "AA101", "Петров П.П.", "2024-10-16"));
    tree.insert(TicketRequest("Омск", "AA103", "Сидоров С.С.", "2024-10-15")); 
    tree.insert(TicketRequest("Москва", "AA104", "Иванов И.И.", "2024-10-15")); 
    tree.insert(TicketRequest("Москва", "AA108", "Иванов И.И.", "2024-10-15"));
    tree.insert(TicketRequest("Москва", "AA103", "Иванов И.И.", "2024-10-15"));
    tree.insert(TicketRequest("Москва", "AA100", "Демидов Д.Д.", "2024-10-15"));
    tree.insert(TicketRequest("Москва", "AA103", "Иванов И.И.", "2023-10-15"));*/

    cout << "все заявки в порядке возрастания:\n";
    tree.displayTickets();

    //tree.removeTicket("AA103", "2024-10-15", "bhbhh И.И.");
    tree.removeTicket("AA104", "2024-10-15", "Иванов И.И.");
    tree.displayTickets();
    tree.removeTicket("AA103", "2024-10-15");

    int flag = tree.SaveOptiones();
    if (flag != 0) {
        cout << "не удалось распечатать дерево" << endl;
    }
    else cout << "дерево распечатано" << endl;

    return 0;
}

