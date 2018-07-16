//
// Created by Jens Nilsson on 2018-06-21.
//

#include <iostream>
#include <vector>       // std::vector

class Signal {
    template <class T>
private:
    std::vector<T> signalVector;
    int freq;
    int sampleRate;
    void fft(std::vector<T> in, int N){

        if (N < 2){}
        else{
            T temp = new T[N/2];
            for (int i = 0; i < N/2; i++){
                temp[i] = in[i];
            }
            //All even elements in the first half of vector
            for ( int i = 0; i < N/2; i++){
                in[i] = in[i*2];
            }
            //All odd elements in the end half of vector
            for ( int i = 0; i < N/4; i++){
                in[i+N/2] = temp[i*2 +1];
                in[i+3*N/2] = in[i*2 + N/2];
            }
        }

    }
public:
    Signal(int freq, int sampleRate){
        this->freq = freq;
        this->sampleRate = sampleRate;
    };
    std::vector<T> getSignalVector(){
        return signalVector;
    };
};

class ConstantSignal: public Signal {

};

int main () {
    int N = 6;
    int test[N];
    test = {1,2,3,4,5,6};
    std::cout << fft(test, N) << std::endl;

}


