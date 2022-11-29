#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//判断是否是自反的
bool 是自反的(vector<vector<bool>> p)
{
    size_t length = p.size();
    bool flag = 1;
    for (int i = 0; i < length; i++)
    {
        if (p[i][i] == 0)
        {
            flag = 0;
        }
        break;
    }
    return flag;
}

//判断是否是反对称的
bool 是反对称的(vector<vector<bool>> p)
{
    size_t length = p.size();
    bool flag = 1;
    for (int m = 0; m < length; m++)
    {
        for (int n = 0; n < m; n++)
        {
            if (p[m][n] == p[n][m] && p[m][n] == 1)
            {
                flag = 0;
                return flag;
            }
        }
    }
    return flag;
}

//判断是否是传递的
bool 是传递的(vector<vector<bool>> p)
{
    size_t length = p.size();
    for (auto i = 0; i < length; i++)
    {
        for (auto j = 0; j < length; j++)
        {
            if (i == j)
            {
                continue;
            }
            if (p[i][j] == 1)
            {
                for (auto g = 0; g < length; g++)
                {
                    if (p[j][g] == 1)
                    {
                        if (p[i][g] != 1)
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

size_t vector_string中寻找(const vector<string> &A, const string &a)
{
    auto t = A.begin();
    for (auto i = 0; i < A.size(); i++)
    {
        if (a == *t)
        {
            return i;
        }
        t++;
    }
    return -1;
}

//利用R_string,填充关系R数组
vector<vector<bool>> 创建关系矩阵(string R_string, vector<string> A)
{
    vector<vector<bool>> p(A.size(), vector<bool>(A.size(), false));
    size_t number = 0;
    for (size_t i = 0; i < R_string.length(); i++)
    {
        string a = "";
        string b = "";
        if (R_string[i] == '<')
        {
            i++;
            while (R_string[i] != ',' && i < R_string.length())
            {
                a += R_string[i];
                i++;
            }
            i++;
            while (R_string[i] != '>' && i < R_string.length())
            {
                b += R_string[i];
                i++;
            }
            p[vector_string中寻找(A, a)][vector_string中寻找(A, b)] = 1;
            number++;
        }
    }
    return p;
}

vector<string> 创建元素集合(string A_input)
{
    vector<string> A;
    for (size_t i = A_input.find('{') + 1; i < A_input.length(); i++) //获取集合元素数组X
    {
        string x = "";
        while (A_input[i] != ',' && A_input[i] != '}')
        {
            x += A_input[i];
            i++;
        }
        A.push_back(x);
    }
    sort(A.begin(), A.end());                     //排序,方便后面去重
    A.erase(unique(A.begin(), A.end()), A.end()); //去除重复
    return A;
}

void 获取并打印盖住关系(vector<vector<bool>> p, vector<string> A)
{
    for (auto n = 0; n < A.size(); n++)
    {
        for (auto m = 0; m < A.size(); m++)
        {
            if (p[n][m] == false || n == m)
            {
                continue;
            }
            bool flag = true;
            for (auto i = 0; i < A.size(); i++)
            {
                if (i == m || i == n)
                {
                    continue;
                }
                if (p[n][i] & p[i][m])
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                cout << "<" << A[n] << "," << A[m] << ">"
                     << ",";
            }
        }
    }
    cout << "\n";
}

size_t 交运算(vector<vector<bool>> p, size_t a, size_t b)
{
    size_t length = p.size();
    vector<int> 小于交集;
    for (auto i = 0; i < length; i++)
    {
        if (p[i][a] && p[i][b])
        {
            小于交集.push_back(i);
        }
    }
    for (const auto &i : 小于交集)
    {
        bool flag = true;
        for (const auto &j : 小于交集)
        {
            if (p[j][i] == 0)
            {
                flag = false;
                break;
            }
        }
        if (flag == true)
        {
            return i;
        }
    }
    return -1;
}

size_t 并运算(vector<vector<bool>> p, size_t a, size_t b)
{
    size_t length = p.size();
    vector<int> 大于交集;
    for (auto i = 0; i < length; i++)
    {
        if (p[a][i] && p[b][i])
        {
            大于交集.push_back(i);
        }
    }
    for (const auto &i : 大于交集)
    {
        bool flag = true;
        for (const auto &j : 大于交集)
        {
            if (p[i][j] == 0)
            {
                flag = false;
                break;
            }
        }
        if (flag == true)
        {
            return i;
        }
    }
    return -1;
}

//判断是否为格
bool 是格(vector<vector<bool>> p)
{
    if (!是传递的(p) || !是反对称的(p) || !是自反的(p))
    {
        return false;
    }
    //判断是否任意两个元素有最大下界和最大上界
    {
        for (size_t i = 0; i < p.size(); i++)
        {
            for (size_t j = p.size() - 1; j > i; j--)
            {
                if (交运算(p, i, j) == -1 || 并运算(p, i, j) == -1)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

size_t 获取上确界(vector<vector<bool>> p)
{
    size_t length = p.size();
    size_t b = -1;
    for (auto i = 0; i < length; i++)
    {
        auto j = 0;
        for (; j < length; j++)
        {
            if (p[j][i] == 0)
            {
                break;
            }
        }
        if (j == length)
        {
            b = i;
            return b;
        }
    }
    return b;
}

size_t 获取下确界(vector<vector<bool>> p)
{
    size_t length = p.size();
    size_t a = -1;
    for (auto i = 0; i < length; i++)
    {
        auto j = 0;
        for (; j < length; j++)
        {
            if (p[i][j] == 0)
            {
                break;
            }
        }
        if (j == length)
        {
            a = i;
            return a;
        }
    }
    return a;
}

//判断是否为有补格
bool 是有补格(vector<vector<bool>> p)
{
    if (!是格(p))
    {
        return false;
    }
    size_t 上界 = 获取上确界(p);
    size_t 下界 = 获取下确界(p);
    if (上界 == -1 || 下界 == -1)
    {
        return false;
    }
    for (size_t i = 0; i < p.size(); i++)
    {
        bool flag = false;
        for (size_t j = 0; j < p.size(); j++)
        {
            if (交运算(p, i, j) == 下界 && 并运算(p, i, j) == 上界)
            {
                flag = true;
                break;
            }
        }
        if (flag == false)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    system("chcp 65001"); //控制控制台的输出编码为utf-8
    string A_input = "";
    string R_string = "";
    cout << "请输入集合A" << endl;
    cin >> A_input;
    cout << "请输入关系R" << endl;
    cin >> R_string;

    vector<string> 集合A = 创建元素集合(A_input);
    vector<vector<bool>> 关系矩阵P = 创建关系矩阵(R_string, 集合A);

    if (!是传递的(关系矩阵P) || !是反对称的(关系矩阵P) || !是自反的(关系矩阵P)) //判断是否为偏序集
    {
        cout << "不是偏序关系,已退出";
        return 0;
    }
    cout << "盖住关系如下:\n";
    获取并打印盖住关系(关系矩阵P, 集合A);

    if (是有补格(关系矩阵P))
    {
        cout << "是有补格" << endl;
    }
    else
    {
        cout << "不是有补格" << endl;
    }

    system("pause"); //防止闪退
    return 0;
}
