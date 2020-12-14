namespace GameEngine
{
	template<typename TValue, TValue Val>
	class ConstantType
	{
	public:
        using ValueType = TValue;
        using Type = ConstantType;

        static constexpr ValueType Value = Val;

		constexpr operator ValueType () const noexcept
		{
			return Value;
		}
	};

	using TrueType = ConstantType<bool, true>;
	using FalseType = ConstantType<bool, false>;

	template<class TBase, class TDerived>
	class IsBaseOf
	{
		template<class T>
		static constexpr TrueType Test (T*);

		template<class T>
		static constexpr FalseType Test (void*);

		template<class T>
		static constexpr T&& Val ();

		using Check = decltype (Test<TBase> (Val<TDerived*> ()));

	public:
		static constexpr bool Result = Check::Value;
    };
}
