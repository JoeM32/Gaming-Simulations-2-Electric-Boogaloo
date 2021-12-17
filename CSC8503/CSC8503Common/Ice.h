#pragma once
#include "..\CSC8503Common\GameObject.h"
namespace NCL {
	namespace CSC8503 {

		class Ice : public GameObject 
		{
		public:
			Ice()
			{
				name = "Ice";
			}
			virtual std::string OnDebug() override;



		};
	}
	
}