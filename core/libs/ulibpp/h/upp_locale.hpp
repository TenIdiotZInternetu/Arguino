#ifndef ULIBPP_UPP_LOCALE_HPP_
#define ULIBPP_UPP_LOCALE_HPP_

/*

	upp_locale.hpp

	Operation with codepages

	Kuba, 2007
	Part of ULIB++

*/

#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <string_view>
#include <cstdint>

namespace ulibpp {

	namespace ulibpp_private_ {

		struct cpdesc {
			const char			*cpname_;
			const char			*cpcmt_;
			char16_t			cpt_[256];
		};

		struct cpinfo {
			typedef std::map<char16_t, unsigned char>  bkmap_type;
			const char16_t		*cpt_;
			const bkmap_type	*bkmap_;
		};

		class cpholder {
		public:
			cpholder(const cpinfo &cpi) : cpi_(cpi) { }

			char16_t getucs2(unsigned char c) const { return cpi_.cpt_[c]; }
			unsigned char getc(char16_t u2) const {		// THROWS!! if not in CP
				std::map<char16_t, unsigned char>::const_iterator ci;
				if((ci=cpi_.bkmap_->find(u2))==cpi_.bkmap_->end())
					throw std::range_error("Character not convertible to UCS2");
				return ci->second;
			}
		private:
			cpinfo				cpi_;
		};

		template <typename US>
		US cp2stdux(const cpholder &cph, const std::string &cps) {
			US u2;
			using CHT = typename US::value_type;
			if(cps.size()<1024)					// small strings
			{
				CHT b[1024];
				CHT *p=b;
				for(std::string::size_type idx=0;idx<cps.size();++idx)
					*p++ = cph.getucs2(cps.at(idx));
				*p = 0;
				u2 = b;				// this works for UCS2
			}
			else
			{
				CHT *b = new CHT[cps.size()+1];
				CHT *p=b;
				for(std::string::size_type idx=0;idx<cps.size();++idx)
					*p++ = cph.getucs2(cps.at(idx));
				*p = 0;
				u2 = b;				// this works for UCS2
				delete[] b;
			}
			return u2;
		}
	}

	// container for holding CP data
	class cp_tables {
	private:
		typedef std::vector<ulibpp_private_::cpinfo>			cpv_type;
		typedef std::map<std::string, cpv_type::size_type>		cpm_type;

		class cp_tables_const_iterator_ {
		public:
			cp_tables_const_iterator_() : ci_() { }
			cp_tables_const_iterator_ &operator++() { ++ci_; return *this; }
			cp_tables_const_iterator_ operator++(int) { cp_tables_const_iterator_ temp(*this); ++*this; return temp; }
			cp_tables_const_iterator_ &operator--() { --ci_; return *this; }
			cp_tables_const_iterator_ operator--(int) { cp_tables_const_iterator_ temp(*this); --*this; return temp; }
			const std::string operator*() const { return ci_->first; }

		private:
			cp_tables_const_iterator_(cpm_type::const_iterator ci) : ci_(ci) { }
			cpm_type::const_iterator		ci_;

			friend class cp_tables;
			friend bool operator==(const cp_tables_const_iterator_ &lhs, const cp_tables_const_iterator_ &rhs);
			friend bool operator!=(const cp_tables_const_iterator_ &lhs, const cp_tables_const_iterator_ &rhs);
		};

	public:
		cp_tables();
		~cp_tables();

		// container requirements
		typedef std::string value_type;
		typedef std::string &reference;
		typedef const std::string &const_reference;
		typedef cp_tables_const_iterator_ const_iterator;
		typedef cpm_type::size_type size_type;

		const_iterator begin() const { return cp_tables_const_iterator_(cpm_.begin()); }
		const_iterator end() const { return cp_tables_const_iterator_(cpm_.end()); }
		void swap(cp_tables &l) {
			std::swap(cpv_, l.cpv_);
			std::swap(cpm_, l.cpm_);
		}
		size_type size() const { return cpm_.size(); }
		bool empty() const { return cpm_.empty(); }
	private:
		ulibpp_private_::cpinfo get_cpinfo(const std::string &cpname);

		cpv_type							cpv_;
		cpm_type							cpm_;

		friend class cp_convertor;
	};

	inline bool operator==(const cp_tables::const_iterator &lhs, const cp_tables::const_iterator &rhs) { return lhs.ci_==rhs.ci_; }
	inline bool operator!=(const cp_tables::const_iterator &lhs, const cp_tables::const_iterator &rhs) { return lhs.ci_!=rhs.ci_; }

	extern cp_tables		cp_tables_manager;  // singleton for managing CPs

	class cp_convertor {
	public:
		cp_convertor(const std::string &cpname) : cph_(cp_tables_manager.get_cpinfo(cpname)) { }   // THROWS!! if unknown codepage name

		std::wstring cp2ws(const std::string &cps) const { return ulibpp_private_::cp2stdux<std::wstring>(cph_, cps); }
		std::u16string cp2u16(const std::string &cps) const { return ulibpp_private_::cp2stdux<std::u16string>(cph_, cps); }
		std::u32string cp2u32(const std::string &cps) const { return ulibpp_private_::cp2stdux<std::u32string>(cph_, cps); }

		std::string ws2cp(const std::wstring &ws) const { 
			std::u16string u2(ws.begin(), ws.end());
			std::string s;
			if(u2.size()<1024)					// small strings
			{
				char b[1024];
				char *p=b;
				for(auto uc : u2)
					*p++ = cph_.getc(uc);		// can THROW!!! if the conversion does not exist
				*p = 0;
				s = b;
			}
			else
			{
				char *b = new char[u2.size()+1];
				char *p=b;
				for(auto uc : u2)
					*p++ = cph_.getc(uc);		// can THROW!!! if the conversion does not exist
				*p = 0;
				s = b;
				delete[] b;
			}
			return s;
		}

	private:
		ulibpp_private_::cpholder				cph_;
	};


	// Unicode conversion functions

	namespace ulibpp_private_ {

		struct utf8_pol {
			using in_type = std::string_view;
			using out_type = std::string;
			static std::uint32_t get_char(const in_type in, std::size_t &pos);
			static void put_char(out_type &out, std::uint32_t c);
		};

		struct utf16_pol {
			using in_type = std::u16string_view;
			using out_type = std::u16string;
			static std::uint32_t get_char(const in_type in, std::size_t &pos);
			static void put_char(out_type &out, std::uint32_t c);
		};

		struct ucs2_pol {
			using in_type = std::u16string_view;
			using out_type = std::u16string;
			static inline std::uint32_t get_char(const in_type in, std::size_t &pos) { return static_cast<std::uint32_t>(in[pos]); }
			static inline void put_char(out_type &out, std::uint32_t c) { if(c>0x10000) throw std::invalid_argument("Character out of UCS2"); out += static_cast<char16_t>(c); }
		};

		struct ucs4_pol {
			using in_type = std::u32string_view;
			using out_type = std::u32string;
			static inline std::uint32_t get_char(const in_type in, std::size_t &pos) { return static_cast<std::uint32_t>(in[pos]); }
			static inline void put_char(out_type &out, std::uint32_t c) { out += static_cast<char32_t>(c); }
		};

		template <std::size_t WCS>
		static void put_char_int_(std::wstring &out, std::uint32_t c);		// not implemented, only specializations
		template <>
		inline void put_char_int_<2>(std::wstring &out, std::uint32_t c) { if(c>0x10000) throw std::invalid_argument("Character out of UCS2"); out += static_cast<wchar_t>(c); }
		template <>
		inline void put_char_int_<4>(std::wstring &out, std::uint32_t c) { out += static_cast<wchar_t>(c); }

		struct wstr_pol {
			using in_type = std::wstring_view;
			using out_type = std::wstring;
			static inline std::uint32_t get_char(const in_type in, std::size_t &pos) { return static_cast<std::uint32_t>(in[pos]); }
			static inline void put_char(out_type &out, std::uint32_t c) { put_char_int_<sizeof(wchar_t)>(out, c); }
		};

		template <typename IPOL, typename OPOL>
		inline typename OPOL::out_type cvrt_utfx2utfx(const typename IPOL::in_type in)
		{
			typename OPOL::out_type rv;
			for(std::size_t pos=0;pos<in.size();++pos)
			{
				std::uint32_t ch = IPOL::get_char(in, pos);
				OPOL::put_char(rv, ch);
			}
			return std::move(rv);
		}
	}

	inline std::wstring cvrt_utf82wstring(const std::string_view u8) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::utf8_pol, ulibpp_private_::wstr_pol>(u8); }
	inline std::u16string cvrt_utf82ucs2(const std::string_view u8) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::utf8_pol, ulibpp_private_::ucs2_pol>(u8); }
	inline std::u16string cvrt_utf82utf16(const std::string_view u8) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::utf8_pol, ulibpp_private_::utf16_pol>(u8); }
	inline std::u32string cvrt_utf82ucs4(const std::string_view u8) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::utf8_pol, ulibpp_private_::ucs4_pol>(u8); }

	inline std::wstring cvrt_ucs22wstring(const std::u16string_view u2) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::ucs2_pol, ulibpp_private_::wstr_pol>(u2); }
	inline std::string cvrt_ucs22utf8(const std::u16string_view u2) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::ucs2_pol, ulibpp_private_::utf8_pol>(u2); }
	inline std::u16string cvrt_ucs22utf16(const std::u16string_view u2) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::ucs2_pol, ulibpp_private_::utf16_pol>(u2); }
	inline std::u32string cvrt_ucs22ucs4(const std::u16string_view u2) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::ucs2_pol, ulibpp_private_::ucs4_pol>(u2); }

	inline std::wstring cvrt_utf162wstring(const std::u16string_view u16) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::utf16_pol, ulibpp_private_::wstr_pol>(u16); }
	inline std::string cvrt_utf162utf8(const std::u16string_view u16) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::utf16_pol, ulibpp_private_::utf8_pol>(u16); }
	inline std::u16string cvrt_utf162ucs2(const std::u16string_view u16) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::utf16_pol, ulibpp_private_::ucs2_pol>(u16); }
	inline std::u32string cvrt_utf162ucs4(const std::u16string_view u16) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::utf16_pol, ulibpp_private_::ucs4_pol>(u16); }

	inline std::wstring cvrt_ucs42wstring(const std::u32string_view u32) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::ucs4_pol, ulibpp_private_::wstr_pol>(u32); }
	inline std::string cvrt_ucs42utf8(const std::u32string_view u32) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::ucs4_pol, ulibpp_private_::utf8_pol>(u32); }
	inline std::u16string cvrt_ucs42ucs2(const std::u32string_view u32) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::ucs4_pol, ulibpp_private_::ucs2_pol>(u32); }
	inline std::u16string cvrt_ucs42utf16(const std::u32string_view u32) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::ucs4_pol, ulibpp_private_::utf16_pol>(u32); }

	inline std::string cvrt_wstring2utf8(const std::wstring_view w) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::wstr_pol, ulibpp_private_::utf8_pol>(w); }
	inline std::u16string cvrt_wstring2ucs2(const std::wstring_view w) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::wstr_pol, ulibpp_private_::ucs2_pol>(w); }
	inline std::u16string cvrt_wstring2utf16(const std::wstring_view w) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::wstr_pol, ulibpp_private_::utf16_pol>(w); }
	inline std::u32string cvrt_wstring2ucs4(const std::wstring_view w) { return ulibpp_private_::cvrt_utfx2utfx<ulibpp_private_::wstr_pol, ulibpp_private_::ucs4_pol>(w); }
}

namespace std {

	template <>
	struct iterator_traits< ::ulibpp::cp_tables::cp_tables_const_iterator_ > {
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef const std::string value_type;
		typedef std::ptrdiff_t difference_type;
		typedef const std::string * pointer;
		typedef const std::string & reference;
	};

}

#endif
