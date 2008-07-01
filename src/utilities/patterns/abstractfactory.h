#ifndef ABSTRACTFACTORY_H_INCLUDED
#define ABSTRACTFACTORY_H_INCLUDED

#include <map>
#include <tr1/memory>

using std::tr1::shared_ptr;

template <typename AbstractProduct,
          typename IdentifierType,
          typename ProductCreator = shared_ptr<AbstractProduct> (*)()>
class AbstractFactory {
	typedef AbstractFactory<AbstractProduct, IdentifierType, ProductCreator> ThisClass;
public:
	shared_ptr<AbstractProduct> create(const IdentifierType & id) {
			typename Associations::const_iterator i = this->associations_.find(id);
			if (this->associations_.end() != i) {
					return (i->second)();
			}
			return shared_ptr<AbstractProduct>();
	}

	bool subscribe(const IdentifierType & id, ProductCreator creator) {
		return this->associations_.insert(typename Associations::value_type(id, creator)).second;
	}

private:
  typedef std::map<IdentifierType, ProductCreator> Associations;
	Associations associations_;
};


template <typename Base, typename Derived>
shared_ptr<Base> createInstance()
{
    return shared_ptr<Base>(new Derived);
}

#endif // ABSTRACTFACTORY_H_INCLUDED
