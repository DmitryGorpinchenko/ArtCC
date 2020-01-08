#include <iostream>
#include <exception>
#include <vector>

using namespace std;

class Matrix {
public:
    Matrix(int rows = 0, int cols = 0) {
        Reset(rows, cols);
    }

    void Reset(int rows, int cols) {
        if (rows < 0 || cols < 0) {
            throw out_of_range("negative dimensions");
        }
        if ((rows == 0) || (cols == 0)) {
            data = {};
        } else {
            data = { size_t(rows), vector<int>(cols, 0) };
        }
    }
    int& At(int row, int col) {
        return data.at(row).at(col);
    }
    int At(int row, int col) const {
        return data.at(row).at(col);
    }
    int GetNumRows() const { return data.size(); }
    int GetNumColumns() const { return (GetNumRows() > 0) ? data.at(0).size() : 0; }
private:
    vector<vector<int>> data;
};

bool operator ==(const Matrix& lhs, const Matrix& rhs) {
    const int rows = lhs.GetNumRows();
    const int cols = lhs.GetNumColumns();
    if ((rows != rhs.GetNumRows()) || (cols != rhs.GetNumColumns())) {
        return false;
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (lhs.At(i, j) != rhs.At(i, j)) {
                return false;
            }
        }
    }
    return true;
}
Matrix operator +(const Matrix& lhs, const Matrix& rhs) {
    const int rows = lhs.GetNumRows();
    const int cols = lhs.GetNumColumns();
    if ((rows != rhs.GetNumRows()) || (cols != rhs.GetNumColumns())) {
        throw invalid_argument("dimensions are different");
    }
    Matrix res(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            res.At(i, j) = lhs.At(i, j) + rhs.At(i, j);
        }
    }
    return res;
}
ostream& operator <<(ostream& out, const Matrix& val) {
    const int rows = val.GetNumRows();
    const int cols = val.GetNumColumns();
    out << rows << ' ' << cols << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            out << val.At(i, j) << ' ';
        }
        out << endl;
    }
    return out;
}
istream& operator >>(istream& in, Matrix& val) {
    int rows = 0, cols = 0;
    in >> rows >> cols;
    val.Reset(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            in >> val.At(i, j);
        }
    }
    return in;
}

int main() {
    Matrix one;
    Matrix two;
    cin >> one >> two;
    cout << one + two << endl;
    return 0;
}
