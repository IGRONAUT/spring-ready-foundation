#include "ProductDatabase.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include <iostream>

using namespace std;
using namespace SQLite;

// Конструктор класу ProductDatabase. Відкриває базу даних SQLite.
ProductDatabase::ProductDatabase(const string& db_filename) {
    try {
        db_ = new Database("ShopDB.sqlite3", OPEN_READWRITE | OPEN_CREATE);
    }
    catch (exception& e) {
        cerr << "<h2>Error opening database: " << e.what() << "</h2>" << endl;
    }
}

// Деструктор класу ProductDatabase. Зачиняє базу даних SQLite.
ProductDatabase::~ProductDatabase() {
    if (db_ != nullptr)
    {
        delete db_;
        db_ = nullptr;
    }
}

// Метод для пошуку категорії за іменем. Повертає категорію, якщо вона знайдена.
Category ProductDatabase::find_category_by_name(const string& category_name) const {
    Statement query(*db_, "SELECT * FROM categories WHERE name LIKE ?;");
    query.bind(1, category_name);

    if (query.executeStep()) {
        Category category;
        category.id = query.getColumn(0).getString();
        category.name = query.getColumn(1).getString();
        return category;
    }

    return Category(); // Повертає порожню категорію, якщо категорія не знайдена
}

// Метод для пошуку товару за моделлю. Повертає товар, якщо він знайдений.
Product ProductDatabase::find_product_by_model(const string& product_model) const {
    Statement query(*db_, "SELECT * FROM products WHERE model LIKE ?;");
    query.bind(1, product_model);

    if (query.executeStep()) {
        Product product;
        product.id = query.getColumn(0).getString();
        product.manufacturer = query.getColumn(1).getString();
        product.model = query.getColumn(2).getString();
        product.description = query.getColumn(3).getString();
        product.price = query.getColumn(4).getDouble();
        product.quantity = query.getColumn(5).getInt();
        product.code = query.getColumn(6).getString();
        product.category_id = query.getColumn(7).getInt();
        return product;
    }

    return Product(); // Повертає порожній товар, якщо товар не знайдено
}

// Метод для пошуку товарів за ключовими словами (модель або виробник). Повертає список товарів, що відповідають ключовим словам.
vector<Product> ProductDatabase::find_products_by_keywords(const string& keywords) const {
    vector<Product> products;
    Statement query(*db_, "SELECT id, manufacturer, model FROM products WHERE model LIKE ? OR manufacturer LIKE ?;");
    string pattern = "%" + keywords + "%"; // Дозволяє шукати будь-які стрічки, що зберігають слово в будь-якому місці
    query.bind(1, pattern); // Прив'язування стрічки pattern до параметру запиту (?)
    query.bind(2, pattern); // bind замінює параметри на конкретні значення

    while (query.executeStep()) {
        Product product;
        product.id = query.getColumn(0).getString();
        product.manufacturer = query.getColumn(1).getString();
        product.model = query.getColumn(2).getString();
        products.push_back(product);
    }

    return products; // Повертає список продуктів, що відповідають ключовим словам
}

// Метод для отримання списку всіх категорій. Повертає список категорій.
vector<Category> ProductDatabase::get_categories() const {
    vector<Category> categories;
    Statement query(*db_, "SELECT id, name FROM categories;");

    while (query.executeStep()) {
        Category category;
        category.id = query.getColumn(0).getString();
        category.name = query.getColumn(1).getString();
        categories.push_back(category);
    }

    return categories;
}

// Метод для отримання списку всіх товарів. Повертає список товарів.
vector<Product> ProductDatabase::get_products() const {
    vector<Product> products;
    Statement query(*db_, "SELECT id, manufacturer, model FROM products;");

    while (query.executeStep()) {
        Product product;
        product.id = query.getColumn(0).getString();
        product.manufacturer = query.getColumn(1).getString();
        product.model = query.getColumn(2).getString();
        products.push_back(product);
    }

    return products;
}

// Метод для додавання нової категорії. Приймає назву категорії.
void ProductDatabase::add_category(const string& category_name) {
    try {
        Statement query(*db_, "INSERT INTO categories (name) VALUES (?);");
        query.bind(1, category_name);

        query.exec();
    }
    catch (const Exception& e) {
        cerr << "<h2>Error executing insert query: " << e.what() << "</h2>" << endl;
    }
}

// Метод для додавання нового товару. Приймає інформацію про товар.
void ProductDatabase::add_product(const string& manufacturer, const string& model,
    const string& description, double price, int quantity,
    const string& code, int category_id) {
    try {
        Statement query(*db_, "INSERT INTO products (manufacturer, model, description, "
            "price, quantity, code, category_id) VALUES (?, ?, ?, ?, ?, ?, ?);");
        query.bind(1, manufacturer);
        query.bind(2, model);
        query.bind(3, description);
        query.bind(4, price);
        query.bind(5, quantity);
        query.bind(6, code);
        query.bind(7, category_id);

        query.exec();
    }
    catch (const Exception& e) {
        cerr << "<h2>Error executing insert query: " << e.what() << "</h2>" << endl;
    }
}

// Метод для видалення категорії. Приймає ідентифікатор категорії.
void ProductDatabase::delete_category(int category_id) {
    try {
        Statement query(*db_, "DELETE FROM categories WHERE id = ?;");
        query.bind(1, category_id);

        query.exec();
    }
    catch (const Exception& e) {
        cerr << "<h2>Error executing delete query: " << e.what() << "</h2>" << endl;
    }
}

// Метод для видалення товару. Приймає ідентифікатор товару.
void ProductDatabase::delete_product(int product_id) {
    try {
        Statement query(*db_, "DELETE FROM products WHERE id = ?;");
        query.bind(1, product_id);

        query.exec();
    }
    catch (const Exception& e) {
        cerr << "<h2>Error executing delete query: " << e.what() << "</h2>" << endl;
    }
}