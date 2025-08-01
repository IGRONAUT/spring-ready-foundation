#pragma once // ��������� ������������� ����� ����� ���� ��� ��� ���������

#include "SQLiteCpp/SQLiteCpp.h"
#include <string>
#include <vector>

using namespace std;
using namespace SQLite;

// ��������� ��� ������������� ������� ������
struct Category {
    string id; // ��������� ������������� �������
    string name; // ����� �������
};

// ��������� ��� ������������� ������
struct Product {
    string id; // ��������� ������������� ������
    string manufacturer; // �������� ������
    string model; // ������ ������
    string description; // ���� ������
    double price; // ֳ�� ������
    int quantity; // ʳ������ ������ �� �����
    string code; // ��� ������
    int category_id; // ������������� �������, �� ��� �������� �����
};

// ���� ��� ������ � ����� ����� ������
class ProductDatabase {
public:
    // ����������� ������ ��'� ����� ���� �����
    ProductDatabase(const string& db_filename);

    // ����������
    ~ProductDatabase();

    // ����� ��� ������ ������� �� ������
    Category find_category_by_name(const string& category_name) const;

    // ����� ��� ������ ������ �� �������
    Product find_product_by_model(const string& product_model) const;

    // ����� ��� ������ ������ �� ��������� �������
    vector<Product> find_products_by_keywords(const string& keywords) const;

    // ����� ��� ��������� ������ ��� ��������
    vector<Category> get_categories() const;

    // ����� ��� ��������� ������ ��� ������
    vector<Product> get_products() const;

    // ����� ��� ��������� ���� �������
    void add_category(const string& category_name);

    // ����� ��� ��������� ������ ������
    void add_product(const string& manufacturer, const string& model,
        const string& description, double price, int quantity,
        const string& code, int category_id);

    // ����� ��� ��������� �������
    void delete_category(int category_id);

    // ����� ��� ��������� ������
    void delete_product(int product_id);

private:
    Database* db_; // �������� �� ��'��� ���� ����� SQLiteCpp
};
