#define _CRT_SECURE_NO_WARNINGS // ³��������� ����������� �������

#include "ProductDatabase.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace SQLite;

string urlDecode(string& SRC) {
    string ret;  // ����������� ������
    char ch;     // ������ ��� ��������� ������������ ��������
    int i, ii;   // ������� ��� �������� �� ������ ������ �� ��������� ������������ ��������

    // �������� �� ������ ������
    for (i = 0; i < SRC.length(); i++) {
        // ���� �������� ������ � URL-��������� �������� (%)
        if (int(SRC[i]) == 37) {
            // ����������� URL-���������� �������
            sscanf(SRC.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
        else {
            // ��������� ��������� ������� �� ����������� ������
            ret += SRC[i];
        }
    }

    // ���������� ����������� ������
    return (ret);
}

int main() {
    // ��������� ��'���� ���� �����
    ProductDatabase ShopDB("ShopDB.sqlite3");

    // ��������� ������ ������
    string request_method = getenv("REQUEST_METHOD");

    int option = 0;  // ����� ��� ��������� �����, �������� �� �����������
    string data; // ����� data ��� ���������� ������� �����

    // ���� ����� ������ - POST, �� ������ ���� �� STDIN
    if (request_method == "POST") {
        string post_data;  // ����� ��� ��������� ������� �����
        getline(cin, post_data); // ������� ��� ������ ������

        // ��������� ������ �� ���� ����-��������
        stringstream ss(post_data);
        string pair;
        while (getline(ss, pair, '&')) {
            // ��������� ���� ����-�������� �� ���� �� ��������
            string key = pair.substr(0, pair.find("="));
            string value = pair.substr(pair.find("=") + 1);

            // ����� '+' �� ������ � �������
            replace(value.begin(), value.end(), '+', ' ');

            // ����������� URL-��������� �������
            value = urlDecode(value);

            // ������� ����� �� ��������
            if (key == "action") {
                option = stoi(value);  // ��������� �������� �����
            }
            else if (key == "data") {
                data = value;  // ��������� �������� �����
            }
        }
    }

    // ��������� HTTP-���������
    cout << "Content-type: text/html\r\n\r\n";

    // ��������� HTML-���� ��� ������ ������� ������� �� ���� ��� �������� �����
    cout << "<html><body>";
    cout << "<h1>Product Database</h1>";
    cout << "<form method='POST' action=''>";
    cout << "<label for='action'>Choose an action:</label><br>";
    cout << "<select id='action' name='action'>";
    cout << "<option value='1'>Find a category</option>";
    cout << "<option value='2'>Find a product</option>";
    cout << "<option value='3'>Find product by keyword</option>";
    cout << "<option value='4'>List of categories</option>";
    cout << "<option value='5'>List of products</option>";
    cout << "<option value='6'>Add category</option>";
    cout << "<option value='7'>Add product</option>";
    cout << "<option value='8'>Delete category</option>";
    cout << "<option value='9'>Delete product</option>";
    cout << "</select><br>";
    cout << "<label for='data'>Enter data:</label><br>";
    cout << "<input type='text' id='data' name='data'><br>";
    cout << "<input type='submit' value='Submit'>";
    cout << "</form>";

    // ��������� HTML-���� ����� ���������� ��������� ��������
    cout << "<html><body>";
    cout << "<h1>Result:</h1>";

    // ����� ����� � �������� ��� ������������
    ofstream outfile("log.txt");

    // ��������, �� �������� ����
    if (outfile.is_open())
    {
        // ����� ����� � ����
        outfile << data;
        outfile.close();  // �������� �����
    }

    // ��������� ������� ����� ������� �������� �� �����
    if (option == 1) {
        // ����� �������
        string category_name = data;

        // ������ ������� ������ �������
        Category category = ShopDB.find_category_by_name(category_name);
        // ��������� ���������� ������
        cout << "<p>ID: " << category.id << ", Name: " << category.name << "</p>";
    }
    else if (option == 2) {
        // ����� ������ �� �������
        string product_model = data;  // ����� ��� ��������� ����� ��������

        // ������ ������� ������ �������� �� �������
        Product product = ShopDB.find_product_by_model(product_model);
        // ��������� ���������� ������
        cout << "<p><br>ID: " << product.id << "<br>Manufacturer: " << product.manufacturer
            << "<br>Model: " << product.model << "<br>Description: " << product.description
            << "<br>Price: " << product.price << "<br>Quantity: " << product.quantity
            << "<br>Code: " << product.code << "<br>Category ID: " << product.category_id
            << "</p>";
    }
    else if (option == 3) {
        // ����� ������ �� ��������� ������� (�������� ��� ������)
        string keyword = data;  // ����� ��� ��������� �������� ���

        // ������ ������� ������ �������� �� ��������� �������
        vector<Product> products = ShopDB.find_products_by_keywords(keyword);
        // ��������� ���������� ������
        cout << "<p>Found products:</p>";
        cout << "<ul>";
        for (const auto& product : products)
        {
            cout << "<li>ID: " << product.id << ", Manufacturer: " << product.manufacturer << ", Model: " << product.model << "</li>";
        }
        cout << "</ul>";
    }
    else if (option == 4) {
        // ��������� ������ ��������
        // ������ ������� ��� ��������� ������ ��������
        vector<Category> categories = ShopDB.get_categories();
        // ��������� ������ ��������
        cout << "<h2>List of categories:</h2>";
        cout << "<ul>";
        for (const auto& category : categories) {
            cout << "<li>ID: " << category.id << ", Name: " << category.name << "</li>";
        }
        cout << "</ul>";
    }
    else if (option == 5) {
        // ��������� ������ ������
        // ������ ������� ��� ��������� ������ ������
        vector<Product> products = ShopDB.get_products();
        // ��������� ������ ������
        cout << "<h2>List of products:</h2>";
        cout << "<ul>";
        for (const auto& product : products) {
            cout << "<li>ID: " << product.id << ", Manufacturer: " << product.manufacturer << ", Model: " << product.model << "</li>";
        }
        cout << "</ul>";
    }
    else if (option == 6) {
        // ��������� ���� �������
        string new_category = data;  // ����� ��� ��������� ����� ���� �������

        // ������ ������� ��� ��������� ���� �������
        ShopDB.add_category(new_category);
        // ��������� ����������� ��� ��������� ���� �������
        cout << "<p>Category added: " << new_category << "</p>";
    }
    else if (option == 7) {
        // ��������� ������ ������
        // ��������� ������� ����� �� ����� ������� �� ��������� �� ��������� ������
        stringstream ss(data);
        string manufacturer; getline(ss, manufacturer, ';');
        string model; getline(ss, model, ';');
        string description; getline(ss, description, ';');
        double price; ss >> price; ss.ignore(); // ����������� ������ � ����� ���� �����
        int quantity; ss >> quantity; ss.ignore(); // ����������� ������ � ����� ���� �����
        string code; getline(ss, code, ';');
        int category_id; ss >> category_id;
        
        model.erase(0, 1); // ��������� ������ ����� ������� �� ����������
        description.erase(0, 1); // ��� ���������� ��������� �� ����

        // ������ ������� ��� ��������� ������ ������
        ShopDB.add_product(manufacturer, model, description, price, quantity, code, category_id);
        // ��������� ����������� ��� ��������� ������ ������
        cout << "<p>Product added: " << "<p><br>Manufacturer: " << manufacturer
            << "<br>Model: " << model << "<br>Description: " << description
            << "<br>Price: " << price << "<br>Quantity: " << quantity
            << "<br>Code: " << code << "<br>Category ID: " << category_id
            << "</p>";
    }
    else if (option == 8) {
        // ��������� �������
        int category_id = stoi(data);  // ����� ��� ��������� ID �������

        // ������ ������� ��� ��������� �������
        ShopDB.delete_category(category_id);
        // ��������� ����������� ��� ��������� �������
        cout << "<p>Category removed. ID: " << category_id << "</p>";
    }
    else if (option == 9) {
        // ��������� ������
        int product_id = stoi(data);  // ����� ��� ��������� ID ������

        // ������ ������� ��� ��������� ������
        ShopDB.delete_product(product_id);
        // ��������� ����������� ��� ��������� ������
        cout << "<p>Product removed. ID: " << product_id << "</p>";
    }

    // �������� HTML-����
    cout << "</body></html>";

    // ���������� ���� ���������� ��������
    return 0;
}
