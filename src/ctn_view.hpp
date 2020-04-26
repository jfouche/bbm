#ifndef CTN_VIEW_HPP
#define CTN_VIEW_HPP

template<class C, class T = typename C::Data>
class const_view
{
public:
    const_view(const C& container) : m_cnt(&container) { }

    typedef typename C::const_iterator const_iterator;

    int size() const { return m_cnt->size(); }

    const_iterator begin() const { return m_cnt->begin(); }
    const_iterator end() const { return m_cnt->end(); }

    const T& at(int index) const { return (*m_cnt)->at(index); }

private:
    const C* m_cnt;
};

template<class C, class T = typename C::Data>
class view
{
public:
    view(C& container) : m_cnt(&container) { }

    typedef typename C::const_iterator const_iterator;
    typedef typename C::iterator iterator;

    int size() const { return m_cnt->size(); }

    const_iterator begin() const { return m_cnt->begin(); }
    const_iterator end() const { return m_cnt->end(); }

    iterator begin() { return m_cnt->begin(); }
    iterator end() { return m_cnt->end(); }

    const T& at(int index) const { return m_cnt->at(index); }
    T& at(int index) { return (*m_cnt)[index]; }

private:
    C* m_cnt;
};

#endif // CTN_VIEW_HPP
