#pragma once

namespace Sphynx
{
	namespace Reflection
	{
		class SPHYNX_API Attribute
		{
		public:
			virtual ~Attribute() = default;
		};
	}
}

// Common attributes
namespace Sphynx
{
	namespace Reflection
	{
		namespace CommonAttribute
		{
			class SPHYNX_API Description : public ::Sphynx::Reflection::Attribute
			{
			public:
				Description(const char* description) :
					m_Description(description) {};

			private:
				const char* m_Description;

			};

			class SPHYNX_API Range : public ::Sphynx::Reflection::Attribute
			{
			public:
				Range(float from, float to) :
					m_From(from),
					m_To(to) 
				{};

			private:
				float m_From;
				float m_To;
			};
		}
	}
}
