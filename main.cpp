#include <cassert>
#include <cstddef>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>


/// @brief Класс разреженной матрицы.
class Matrix
{
private:
    std::map< std::size_t, std::map< std::size_t, int> > data_; ///<  Информация о занятых ячейках.

public:
    Matrix() = default;

    /// @brief Прокси-класс RowProx.
    class RowProxy
    {
    private:
        std::map< std::size_t, std::map< std::size_t, int> >& matrixData_;
        std::size_t rowIndex_;
    public:
        RowProxy( std::map< std::size_t, std::map< std::size_t, int > >& data, std::size_t row ) 
            : matrixData_{ data }
            , rowIndex_{ row } 
        {}

        int& operator[]( std::size_t colIndex )
        {
            if ( matrixData_[ rowIndex_ ].find( colIndex ) == matrixData_[ rowIndex_ ].end() ) 
            {
                matrixData_[ rowIndex_ ][ colIndex ] = -1;
            }
            return matrixData_[ rowIndex_ ][ colIndex ];
        }
    };


    /// @brief Прокси-класс ConstRowProx.
    class ConstRowProxy
    {
    private:
        const std::map< std::size_t, std::map< std::size_t, int> >& matrixData_;
        std::size_t rowIndex_;
    public:
        ConstRowProxy( const std::map< std::size_t, std::map< std::size_t, int> >& data, std::size_t row ) 
            : matrixData_( data )
            , rowIndex_( row ) 
            {}

        int operator[]( std::size_t colIndex ) const
        {
            auto rowIt = matrixData_.find( rowIndex_ );
            if ( rowIt != matrixData_.end() ) 
            {
                auto colIt = rowIt->second.find(colIndex);
                if ( colIt != rowIt->second.end()) 
                {
                    return colIt->second;
                }
            }
            return -1;
        }
    };

    RowProxy operator[]( std::size_t rowIndex ) 
    { 
        return RowProxy( data_, rowIndex );
    }
    ConstRowProxy operator[]( std::size_t rowIndex ) const 
    { 
        return ConstRowProxy( data_, rowIndex );
    }

    std::size_t size() const
    {
        std::size_t count = 0;
        for ( const auto& rowPair : data_ ) 
        {
            for ( const auto& colPair : rowPair.second ) 
            {
                if ( colPair.second != -1 ) 
                count++;
            }
        }
        return count;
    }

    /// @brief Реализация Кастомного Итератора.
    class Iterator 
    {
    private:
        using RowMap = std::map< std::size_t, std::map< std::size_t, int > >;
        const RowMap* matrixData_;
        RowMap::const_iterator rowIt_;
        std::map< std::size_t, int >::const_iterator colIt_;

        // Вспомогательный метод: двигает итератор вперед, пока не найдет элемент != -1
        void SkipEmpty() 
        {
            while ( rowIt_ != matrixData_->end() ) 
            {
                while ( colIt_ != rowIt_->second.end() ) 
                {
                    if ( colIt_->second != -1 ) 
                    {
                        return; // Нашли валидный элемент
                    }
                    ++colIt_;
                }
                ++rowIt_;
                if ( rowIt_ != matrixData_->end() ) 
                {
                    colIt_ = rowIt_->second.begin();
                }
            }
        }

    public:
        Iterator( const RowMap* data, RowMap::const_iterator rIt )
            : matrixData_{ data }
            , rowIt_{rIt } 
        {
            if ( rowIt_ != matrixData_->end() ) 
            {
                colIt_ = rowIt_->second.begin();
                SkipEmpty();
            }
        }

        std::tuple< int, int, int > operator*() const 
        {
            return std::make_tuple( static_cast< int >( rowIt_->first ), 
                                    static_cast< int >( colIt_->first ), 
                                    colIt_->second );
        }

        Iterator& operator++() 
        {
            ++colIt_;
            SkipEmpty();
            return *this;
        }

        bool operator!=(const Iterator& other) const 
        {
            if ( rowIt_ != other.rowIt_) return true;
            if ( rowIt_ == matrixData_->end() ) return false;
            return colIt_ != other.colIt_;
        }
    };

    Iterator begin() const 
    { 
        return Iterator( &data_,  data_.begin());
    }
    
    Iterator end() const 
    { 
        return Iterator( &data_,  data_.end()); 
    }
};

int main()
{
    Matrix matrix;  
    assert( matrix.size() == 0 ); 
    
    auto a = matrix[ 0 ][ 0 ];  
    assert( a == -1 );  
    assert( matrix.size() == 0 );  

    matrix[ 100 ][ 100 ] = 314;  
    assert( matrix[ 100 ][ 100 ] == 314 );  
    assert( matrix.size() == 1 );  

    for( auto c : matrix )  
    {  
        int x;  
        int y;  
        int v;  
        std::tie( x, y, v ) = c;  
        std::cout << x << " " << y << " " << v << std::endl;  
    } 

    return 0;
}
