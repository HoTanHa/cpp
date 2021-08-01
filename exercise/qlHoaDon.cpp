#include <iostream>
#include <vector>

class Receipt;
int calc_bill(const Receipt &receipt);

class Customer
{
public:
    char name[50];
    char address[50];
    char device_id[50];
    std::vector<Receipt *> receipts;

    friend std::ostream &operator<<(std::ostream &, const Customer &);
};

std::ostream &operator<<(std::ostream &ostr, const Customer &cust)
{
    ostr << "Ho ten: " << cust.name << '\n';
    ostr << "Dia chi: " << cust.address << '\n';
    ostr << "Ma so cong to dien: " << cust.device_id << '\n';
    int bill = 0;
    for (Receipt const *const receipt : cust.receipts)
    {
        bill += calc_bill(*receipt);
    }
    ostr << "Tong tien cac hoa don: " << bill;
    return ostr;
}

Customer input_customer()
{
    Customer cust;
    std::cout << "Ho va ten: ";
    std::cin >> cust.name;
    std::cout << "So Nha: ";
    std::cin >> cust.address;
    std::cout << "Ma so cong to dien: ";
    std::cin >> cust.device_id;
    return cust;
}

class Receipt
{
public:
    Receipt(Customer &customer, int last_number, int current_number)
        : customer_(&customer),
          last_(last_number),
          current_(current_number),
          usage_(current_number - last_number)
    {
    }
    Customer &customer()
    {
        return *customer_;
    }
    int last_number() const
    {
        return last_;
    }
    int current_number() const
    {
        return current_;
    }
    int usage() const
    {
        return usage_;
    }

private:
    Customer *customer_;
    int last_;
    int current_;
    int usage_;
};

Receipt input_receipt(Customer &customer)
{
    int last, current;
    std::cout << "Nhap so dien cu:";
    std::cin >> last;
    std::cout << "Nhap so dien moi:";
    std::cin >> current;

    return Receipt(customer, last, current);
}

int calc_bill(const Receipt &receipt)
{
    const int usage = receipt.usage();
    if (usage <= 0)
    {
        return 0;
    }
    if (usage < 50)
    {
        return usage * 1250;
    }
    else if (usage >= 50 && usage <= 100)
    {
        return usage * 1500;
    }
    else /*(usage > 100)*/
    {
        return usage * 2000;
    }
}

int main()
{
    int n_customer;
    std::vector<Customer> customer_list;
    std::vector<Receipt> receipt_list;
    std::cout << "Nhap so luong khach hang:";
    std::cin >> n_customer;
    std::cout << std::endl;
    //customer_list.reserve(n_customer);

    for (int i = 0; i < n_customer; ++i)
    {
        customer_list.push_back(input_customer());
        Customer &cust = customer_list.back();

        int n_receipt;
        std::cout << "Nhap so luong hoa don:";
        std::cin >> n_receipt;

        if (n_receipt <= 0)
        {
            continue;
        }

        //receipt_list.reserve(receipt_list.capacity() + n_receipt);
        for (int e = 0; e < n_receipt; ++e)
        {
            receipt_list.push_back(input_receipt(cust));
            Receipt &receipt = receipt_list.back();
            cust.receipts.push_back(&receipt);
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "Danh sach khach hang:";
    for (const Customer &cust : customer_list)
    {
        std::cout << cust << "\n=============\n";
    }
    std::cout << "\nTong tat ca cac hoa don: ";
    int total = 0;
    for (const Receipt &receipt : receipt_list)
    {
        total += calc_bill(receipt);
    }
    std::cout << total << '\n';
}