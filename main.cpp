#include <iostream>
#include <vector>
static const int p = 37;
static const int q = 53;
static const int mod = 655360001;
using IntMatrix = std::vector<std::vector<int>>;

class Picture {
public:
    Picture(int n, int m): rows_(n), columns_(m), pixels_(IntMatrix(n, std::vector<int>(m, 0))),
                            hashed_prefixes_(IntMatrix(n, std::vector<int>(m, 0))) {};
    void ReadPicture();
    void PrintOccurances(const Picture& subpicture) const;
private:
    int rows_;
    int columns_;
    IntMatrix pixels_;
    IntMatrix hashed_prefixes_;
    void hash_prefixes() ;
    int hash_part(int i, int j, int rows, int columns, int power_p, int power_q) const;
    int hash() const;
    bool check(const Picture& subpicture, int i, int j) const;

};

int power(int a, int n) {
    int result = 1;
    while (n)
        if (n & 1) {
            result *= a;
            --n;
        }
        else {
            a *= a;
            n >>= 1;
        }
    return result;
}

int main() {
    int n, m, small_n, small_m;
    std::cout << "Введите размеры полной картинки\n";
    std::cin >> n >> m;
    Picture picture(n, m);
    picture.ReadPicture();
    std::cout << "Введите размеры картинки-'вырезки'\n";
    std::cin >> small_n >> small_m;
    Picture subpicture(small_n, small_m);
    subpicture.ReadPicture();

    picture.PrintOccurances(subpicture);
}

//=================Picture=================
//==================Read===================
void Picture::ReadPicture() {
    std::cout << "Введите цвета пикселей\n";
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < columns_; ++j) {
            std::cin >> pixels_[i][j];
        }
    }
    hash_prefixes();
}

//============Finding Subpicture==========
void Picture::PrintOccurances(const Picture& subpicture) const {
    int hashed_subpicture = subpicture.hash();
    int power_p = power(p, subpicture.rows_);
    int power_q = power(q, subpicture.columns_);
    std::cout << "Координаты вхождений (позиция левого верхнего угла 'вырезки' в 0-индексации):\n";
    for (int i = 0; i < rows_ - subpicture.rows_ + 1; ++i) {
        for (int j = 0; j < columns_ - subpicture.columns_ + 1; ++j) {
            int hashed_part = hash_part(i, j, subpicture.rows_, subpicture.columns_, power_p, power_q);
            if (hashed_part == hashed_subpicture && check(subpicture, i, j)) {
                std::cout << i << ' ' << j << std::endl;
            }
        }
    }
}

//===========Hashing==============
void Picture::hash_prefixes() {
    hashed_prefixes_[0][0] = pixels_[0][0];
    for (int i = 1; i < rows_; ++i) {
        hashed_prefixes_[i][0] = (hashed_prefixes_[i - 1][0] * p % mod + pixels_[i][0]) % mod;
    }

    for (int j = 1; j < columns_; ++j) {
        hashed_prefixes_[0][j] = (hashed_prefixes_[0][j - 1] * q % mod + pixels_[0][j]) % mod;
    }

    for (int i = 1; i < rows_; ++i) {
        for (int j = 1; j < columns_; ++j) {
            hashed_prefixes_[i][j] = (hashed_prefixes_[i - 1][j] * p % mod + hashed_prefixes_[i][j - 1] * q % mod -
                    hashed_prefixes_[i - 1][j - 1] * p * q % mod + pixels_[i][j] + mod) % mod;
        }
    }
}

int Picture::hash() const {
    int power_p = 1;
    int hash = 0;
    for (int i = rows_ - 1; i >= 0; --i) {
        int power_q = 1;
        for (int j = columns_ - 1; j >= 0; --j) {
            hash += pixels_[i][j] * power_p * power_q % mod;
            hash %= mod;
            power_q *= q;
            power_q %= mod;
        }
        power_p *= p;
        power_p %= mod;
    }
    return hash;
}

int Picture::hash_part(int i, int j, int rows, int columns, int power_p, int power_q) const {
    if (i != 0 && j != 0) {
        return (hashed_prefixes_[i + rows - 1][j + columns - 1] % mod - hashed_prefixes_[i - 1][j + columns - 1] * power_p % mod -
                hashed_prefixes_[i + rows - 1][j - 1] * power_q % mod + hashed_prefixes_[i - 1][j - 1] * power_p * power_q % mod + mod) % mod;
    } else if (i == 0 && j == 0) {
        return hashed_prefixes_[rows - 1][columns - 1];
    } else if (i == 0) {
        return (hashed_prefixes_[i + rows - 1][j + columns - 1] % mod -
                hashed_prefixes_[i + rows - 1][j - 1] * power_q % mod + mod) % mod;
    } else {
        return (hashed_prefixes_[i + rows - 1][j + columns - 1] % mod -
                hashed_prefixes_[i - 1][j + columns - 1] * power_p % mod + mod) % mod;
    }
}

bool Picture::check(const Picture& subpicture, int i, int j) const {
    for (int a = 0; a < subpicture.rows_; ++a) {
        for (int b = 0; b < subpicture.columns_; ++b) {
            if (subpicture.pixels_[a][b] != pixels_[i + a][j + b]) {
                return false;
            }
        }
    }
    return true;
}
