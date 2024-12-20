#define NOMINMAX
#include <iostream>
#include <string>
#include <map>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <windows.h>

enum class ServiceType {
    Water,
    Electricity,
    Gas
};

class Tariff {
public:
    double getCost(ServiceType service) const {
        switch (service) {
        case ServiceType::Water: return 5.0;
        case ServiceType::Electricity: return 10.0;
        case ServiceType::Gas: return 7.0;
        default: return 0.0;
        }
    }
};

class Resident {
private:
    std::string name;
    std::map<ServiceType, double> services;
    static const int MAX_SERVICES = 5;

public:
    Resident() : name("") {}
    Resident(const std::string& name) : name(name) {}

    bool addService(ServiceType service, double amount) {
        if (services.size() >= MAX_SERVICES) {
            return false;
        }
        services[service] = amount;
        return true;
    }

    double getTotalCost(const Tariff& tariff) const {
        double totalCost = 0.0;
        for (const auto& service : services) {
            totalCost += tariff.getCost(service.first) * service.second;
        }
        return totalCost;
    }

    const std::string& getName() const { return name; }
};

class HousingManagementSystem {
private:
    std::map<std::string, Resident> residents;
    Tariff tariff;

public:
    void addResident(const std::string& name) {
        residents[name] = Resident(name);
    }

    bool residentExists(const std::string& name) const {
        return residents.find(name) != residents.end();
    }

    void addService(const std::string& name, ServiceType service, double amount) {
        if (residentExists(name)) {
            if (residents[name].addService(service, amount)) {
                std::cout << "Услуга добавлена успешно.\n";
            }
            else {
                std::cout << "Ошибка: Превышено максимальное количество услуг для жителя \"" << name << "\".\n";
            }
        }
        else {
            std::cout << "Ошибка: Житель с именем \"" << name << "\" не найден.\n";
        }
    }

    void displayResidentBill(const std::string& name) const {
        if (residentExists(name)) {
            const auto& resident = residents.at(name);
            std::cout << "Счет для " << resident.getName() << ":\n";
            std::cout << "Общая стоимость: $" << resident.getTotalCost(tariff) << "\n";
        }
        else {
            std::cout << "Ошибка: Житель с именем \"" << name << "\" не найден.\n";
        }
    }
};

void displayMenu() {
    std::cout << "1. Добавить жителя\n"
        << "2. Добавить услугу\n"
        << "3. Получить счет жителя\n"
        << "4. Выход\n";
}

bool isNumber(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    HousingManagementSystem hms;

    while (true) {
        displayMenu();
        std::string choiceStr;
        int choice;
        std::cout << "Выберите действие: ";
        std::getline(std::cin, choiceStr);

        if (!isNumber(choiceStr) || choiceStr.length() != 1) {
            std::cout << "Ошибка: Введите число от 1 до 4.\n";
            continue;
        }
        choice = std::stoi(choiceStr);

        switch (choice) {
        case 1: {
            std::string name;
            std::cout << "Введите имя жителя: ";
            std::getline(std::cin, name);
            if (name.empty()) {
                std::cout << "Ошибка: Имя не может быть пустым.\n";
            }
            else {
                hms.addResident(name);
                std::cout << "Житель добавлен успешно.\n";
            }
            break;
        }
        case 2: {
            std::string name;
            std::cout << "Введите имя жителя: ";
            std::getline(std::cin, name);

            if (!hms.residentExists(name)) {
                std::cout << "Ошибка: Житель с именем \"" << name << "\" не найден.\n";
                break;
            }

            int serviceChoice;
            while (true) {
                std::cout << "Введите тип услуги (0 - Вода, 1 - Электричество, 2 - Газ): ";
                std::cin >> serviceChoice;

                if (std::cin.peek() != '\n') {
                    std::cout << "Ошибка: Неверный ввод. Пожалуйста, введите только число.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }

                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (serviceChoice < 0 || serviceChoice > 2) {
                    std::cout << "Ошибка: Неверный выбор услуги. Попробуйте снова.\n";
                }
                else {
                    break;
                }
            }

            ServiceType service = static_cast<ServiceType>(serviceChoice);
            std::string amountStr;
            double amount;
            std::cout << "Введите количество (1 - 100): ";
            while (true) {
                std::getline(std::cin, amountStr);

                if (!isNumber(amountStr) || std::stod(amountStr) <= 0 || std::stod(amountStr) > 100) {
                    std::cout << "Ошибка: Количество должно быть положительным числом и не превышать 100. Попробуйте снова.\n";
                }
                else {
                    amount = std::stod(amountStr);
                    break;
                }
            }

            hms.addService(name, service, amount);
            break;
        }
        case 3: {
            std::string name;
            std::cout << "Введите имя жителя: ";
            std::getline(std::cin, name);
            hms.displayResidentBill(name);
            break;
        }
        case 4: {
            return 0;
        }
        default:
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }

    return 0;
}
