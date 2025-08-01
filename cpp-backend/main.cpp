#define _CRT_SECURE_NO_WARNINGS // Відключення попереджень безпеки

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
    string ret;  // Результуюча стрічка
    char ch;     // Символ для зберігання декодованого значення
    int i, ii;   // Індекси для ітерації по вхідній стрічці та зберігання декодованого значення

    // Ітерація по вхідній стрічці
    for (i = 0; i < SRC.length(); i++) {
        // Якщо поточний символ є URL-кодованим символом (%)
        if (int(SRC[i]) == 37) {
            // Декодування URL-кодованого символу
            sscanf(SRC.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
        else {
            // Додавання поточного символу до результуючої стрічки
            ret += SRC[i];
        }
    }

    // Повернення результуючої стрічки
    return (ret);
}

int main() {
    // Створення об'єкту бази даних
    ProductDatabase ShopDB("ShopDB.sqlite3");

    // Отримання методу запиту
    string request_method = getenv("REQUEST_METHOD");

    int option = 0;  // Змінна для зберігання опції, отриманої від користувача
    string data; // Змінна data для збереження вхідних даних

    // Якщо метод запиту - POST, то читаємо данні із STDIN
    if (request_method == "POST") {
        string post_data;  // Змінна для зберігання вхідних даних
        getline(cin, post_data); // Зчитуємо всю строку цілком

        // Розділення строки на пари ключ-значення
        stringstream ss(post_data);
        string pair;
        while (getline(ss, pair, '&')) {
            // Розділення пари ключ-значення на ключ та значення
            string key = pair.substr(0, pair.find("="));
            string value = pair.substr(pair.find("=") + 1);

            // Заміна '+' на пробіли в значенні
            replace(value.begin(), value.end(), '+', ' ');

            // Декодування URL-кодованих символів
            value = urlDecode(value);

            // Обробка ключа та значення
            if (key == "action") {
                option = stoi(value);  // Присвоєння значення опції
            }
            else if (key == "data") {
                data = value;  // Присвоєння значення даним
            }
        }
    }

    // Виведення HTTP-заголовків
    cout << "Content-type: text/html\r\n\r\n";

    // Виведення HTML-коду для вибору потрібної функції та поле для введення даних
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

    // Виведення HTML-коду блоку результатів виконання програми
    cout << "<html><body>";
    cout << "<h1>Result:</h1>";

    // Запис даних з браузера для відлагодження
    ofstream outfile("log.txt");

    // Перевірка, чи відкритий файл
    if (outfile.is_open())
    {
        // Запис даних у файл
        outfile << data;
        outfile.close();  // Закриття файлу
    }

    // Виконання функцій згідно обраним варіантам та даним
    if (option == 1) {
        // Пошук категорії
        string category_name = data;

        // Виклик функції пошуку категорії
        Category category = ShopDB.find_category_by_name(category_name);
        // Виведення результатів пошуку
        cout << "<p>ID: " << category.id << ", Name: " << category.name << "</p>";
    }
    else if (option == 2) {
        // Пошук товару за моделлю
        string product_model = data;  // Змінна для зберігання моделі продукту

        // Виклик функції пошуку продукту за моделлю
        Product product = ShopDB.find_product_by_model(product_model);
        // Виведення результатів пошуку
        cout << "<p><br>ID: " << product.id << "<br>Manufacturer: " << product.manufacturer
            << "<br>Model: " << product.model << "<br>Description: " << product.description
            << "<br>Price: " << product.price << "<br>Quantity: " << product.quantity
            << "<br>Code: " << product.code << "<br>Category ID: " << product.category_id
            << "</p>";
    }
    else if (option == 3) {
        // Пошук товару за ключовими словами (виробник або модель)
        string keyword = data;  // Змінна для зберігання ключових слів

        // Виклик функції пошуку продуктів за ключовими словами
        vector<Product> products = ShopDB.find_products_by_keywords(keyword);
        // Виведення результатів пошуку
        cout << "<p>Found products:</p>";
        cout << "<ul>";
        for (const auto& product : products)
        {
            cout << "<li>ID: " << product.id << ", Manufacturer: " << product.manufacturer << ", Model: " << product.model << "</li>";
        }
        cout << "</ul>";
    }
    else if (option == 4) {
        // Отримання списку категорій
        // Виклик функції для отримання списку категорій
        vector<Category> categories = ShopDB.get_categories();
        // Виведення списку категорій
        cout << "<h2>List of categories:</h2>";
        cout << "<ul>";
        for (const auto& category : categories) {
            cout << "<li>ID: " << category.id << ", Name: " << category.name << "</li>";
        }
        cout << "</ul>";
    }
    else if (option == 5) {
        // Отримання списку товарів
        // Виклик функції для отримання списку товарів
        vector<Product> products = ShopDB.get_products();
        // Виведення списку товарів
        cout << "<h2>List of products:</h2>";
        cout << "<ul>";
        for (const auto& product : products) {
            cout << "<li>ID: " << product.id << ", Manufacturer: " << product.manufacturer << ", Model: " << product.model << "</li>";
        }
        cout << "</ul>";
    }
    else if (option == 6) {
        // Додавання нової категорії
        string new_category = data;  // Змінна для зберігання назви нової категорії

        // Виклик функції для додавання нової категорії
        ShopDB.add_category(new_category);
        // Виведення повідомлення про додавання нової категорії
        cout << "<p>Category added: " << new_category << "</p>";
    }
    else if (option == 7) {
        // Додавання нового товару
        // Розділення вхідних даних на окремі частини та присвоєння їх відповідним змінним
        stringstream ss(data);
        string manufacturer; getline(ss, manufacturer, ';');
        string model; getline(ss, model, ';');
        string description; getline(ss, description, ';');
        double price; ss >> price; ss.ignore(); // Ігнорування крапки з комою після числа
        int quantity; ss >> quantity; ss.ignore(); // Ігнорування крапки з комою після числа
        string code; getline(ss, code, ';');
        int category_id; ss >> category_id;
        
        model.erase(0, 1); // Виделення пробілу перед моделлю та виробником
        description.erase(0, 1); // для коректного додавання до бази

        // Виклик функції для додавання нового товару
        ShopDB.add_product(manufacturer, model, description, price, quantity, code, category_id);
        // Виведення повідомлення про додавання нового товару
        cout << "<p>Product added: " << "<p><br>Manufacturer: " << manufacturer
            << "<br>Model: " << model << "<br>Description: " << description
            << "<br>Price: " << price << "<br>Quantity: " << quantity
            << "<br>Code: " << code << "<br>Category ID: " << category_id
            << "</p>";
    }
    else if (option == 8) {
        // Видалення категорії
        int category_id = stoi(data);  // Змінна для зберігання ID категорії

        // Виклик функції для видалення категорії
        ShopDB.delete_category(category_id);
        // Виведення повідомлення про видалення категорії
        cout << "<p>Category removed. ID: " << category_id << "</p>";
    }
    else if (option == 9) {
        // Видалення товару
        int product_id = stoi(data);  // Змінна для зберігання ID товару

        // Виклик функції для видалення товару
        ShopDB.delete_product(product_id);
        // Виведення повідомлення про видалення товару
        cout << "<p>Product removed. ID: " << product_id << "</p>";
    }

    // Закриття HTML-тегів
    cout << "</body></html>";

    // Повернення коду завершення програми
    return 0;
}
