#pragma once // Включення заголовкового файлу тільки один раз при компіляції

#include "SQLiteCpp/SQLiteCpp.h"
#include <string>
#include <vector>

using namespace std;
using namespace SQLite;

// Структура для представлення категорії товарів
struct Category {
    string id; // Унікальний ідентифікатор категорії
    string name; // Назва категорії
};

// Структура для представлення товарів
struct Product {
    string id; // Унікальний ідентифікатор товару
    string manufacturer; // Виробник товару
    string model; // Модель товару
    string description; // Опис товару
    double price; // Ціна товару
    int quantity; // Кількість товару на складі
    string code; // Код товару
    int category_id; // Ідентифікатор категорії, до якої належить товар
};

// Клас для роботи з базою даних товарів
class ProductDatabase {
public:
    // Конструктор приймає ім'я файлу бази даних
    ProductDatabase(const string& db_filename);

    // Деструктор
    ~ProductDatabase();

    // Метод для пошуку категорії за іменем
    Category find_category_by_name(const string& category_name) const;

    // Метод для пошуку товару за моделлю
    Product find_product_by_model(const string& product_model) const;

    // Метод для пошуку товарів за ключовими словами
    vector<Product> find_products_by_keywords(const string& keywords) const;

    // Метод для отримання списку всіх категорій
    vector<Category> get_categories() const;

    // Метод для отримання списку всіх товарів
    vector<Product> get_products() const;

    // Метод для додавання нової категорії
    void add_category(const string& category_name);

    // Метод для додавання нового товару
    void add_product(const string& manufacturer, const string& model,
        const string& description, double price, int quantity,
        const string& code, int category_id);

    // Метод для видалення категорії
    void delete_category(int category_id);

    // Метод для видалення товару
    void delete_product(int product_id);

private:
    Database* db_; // Покажчик на об'єкт бази даних SQLiteCpp
};
