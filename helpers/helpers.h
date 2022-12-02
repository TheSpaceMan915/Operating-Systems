#ifndef LAB4_TASK1_CPP_HELPERS_H
#define LAB4_TASK1_CPP_HELPERS_H


template <typename T>
void printArr(int size,const T arr[])
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;
}


template <typename T>
void printVector(const std::vector<T>& arr)
{
    for (T temp : arr)
    {
        std::cout << temp << ' ';
    }
    std::cout << std::endl;
}

#endif //LAB4_TASK1_CPP_HELPERS_H
