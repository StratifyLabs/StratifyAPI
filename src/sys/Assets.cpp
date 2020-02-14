/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2017 Tyler Gilbert; All Rights Reserved

#include <limits.h>

#include "sys/Sys.hpp"
#include "fs/Dir.hpp"
#include "var/Token.hpp"
#include "sys/Assets.hpp"

#include "sys/requests.h"
#include "sgfx/FileFont.hpp"

using namespace sys;
using namespace sgfx;

var::Vector<sgfx::FontInfo> Assets::m_font_info_list;
var::Vector<sgfx::IconFontInfo> Assets::m_icon_font_info_list;
bool Assets::m_is_initialized = false;

int Assets::initialize(){
	//search for fonts
	if( m_is_initialized ){ return 0; }

	find_fonts_in_directory("/assets");
	find_fonts_in_directory("/home");
	find_fonts_in_directory("/home/assets");

	//sort fonts
	m_font_info_list.sort(FontInfo::ascending_style);
	m_font_info_list.sort(FontInfo::ascending_point_size);

	//search for icons
	find_icons_in_directory("/assets");
	find_icons_in_directory("/home");
	find_icons_in_directory("/home/assets");

	m_icon_font_info_list.sort(IconFontInfo::ascending_point_size);

	m_is_initialized = true;
	return 0;
}

void Assets::find_fonts_in_directory(const var::String & path){
	var::Vector<var::String> file_list;
	file_list = fs::Dir::read_list(path);
	for(const auto & entry: file_list){
		if( fs::File::suffix(entry) == "sbf" ){
			m_font_info_list.push_back(
						FontInfo(path + "/" + entry)
						);
		}
	}
}

void Assets::find_icons_in_directory(const var::String & path){
	var::Vector<var::String> file_list;
	file_list = fs::Dir::read_list(path);
	for(const auto & entry: file_list){
		if( fs::File::suffix(entry) == "sbi" ){
			m_icon_font_info_list.push_back(
						IconFontInfo(path + "/" + entry)
						);
		}
	}
}

const sgfx::IconFontInfo * Assets::find_icon_font(
		const sgfx::IconFontInfo::Name name,
		const sgfx::IconFontInfo::PointSize point_size,
		const sgfx::IconFontInfo::IsExactMatch is_exact_match
		){

	initialize();

	const var::String & icon_font_name = name.argument();

	u8 closest_point_size = 0;

	//find point size and weight
	for(auto & info: m_icon_font_info_list){
		if( icon_font_name == info.name() ){

			if( info.point_size() == point_size.argument() ){
				//exact match
				if( info.icon_font() == nullptr ){
					info.icon_font_file().open(info.path(), fs::OpenFlags::read_only());
					info.set_icon_font(new IconFont(info.icon_font_file()));
				}
				return &info;
			}

			if( info.point_size() <= point_size.argument() ){
				if( info.point_size() >= closest_point_size ){
					closest_point_size = info.point_size();
				}
			}
		}

	}

	//could not find an exact match
	if( is_exact_match.argument() ){
		return nullptr;
	}

	//first pass is to find the exact style in a point size that is less than or equal
	for(auto & info: m_icon_font_info_list){
		if( icon_font_name == info.name() ){
			if( info.point_size() == closest_point_size ){
				if( info.icon_font() == nullptr ){
					info.icon_font_file().open(info.path(), fs::OpenFlags::read_only());
					info.set_icon_font(new IconFont(info.icon_font_file()));
				}
				return &info;
			}
		}
	}
	return nullptr;
}

const sgfx::FontInfo * Assets::find_font(
		const sgfx::FontInfo::Name name,
		const sgfx::FontInfo::PointSize point_size,
		const sgfx::FontInfo::Style style,
		const sgfx::FontInfo::IsExactMatch is_exact_match
		){

	initialize();

	const var::String & font_name = name.argument();

	u8 closest_point_size = 0;
	u8 closest_style = 0;

	//find point size and weight
	for(u32 i=0; i < font_info_list().count(); i++){
		sgfx::FontInfo & info = m_font_info_list.at(i);
		if( ((style.argument() == FontInfo::style_icons) &&
				 (info.style() == FontInfo::style_icons))
				||
				((style.argument() != FontInfo::style_icons) &&
				 (info.style() != FontInfo::style_icons)) ){

			if( font_name.is_empty() || (font_name == info.name()) ){
				if( info.point_size() <= point_size.argument() ){
					if( info.point_size() >= closest_point_size ){
						closest_point_size = info.point_size();
						closest_style = info.style();
					}
				}

				if( (info.style() == style.argument()) &&
						(info.point_size() == point_size.argument()) &&
						(info.name() == name.argument() || name.argument().is_empty()) ){
					//exact match
					if( info.font() == 0 ){
						info.set_font(new FileFont(info.path()));
						info.font()->set_space_size( info.font()->get_height() / 4);
					}
					return &info;
				}
			}
		}
	}

	//could not find an exact match
	if( is_exact_match.argument() ){
		return nullptr;
	}

	//first pass is to find the exact style in a point size that is less than or equal
	for(u32 i=0; i < font_info_list().count(); i++){
		sgfx::FontInfo & info(m_font_info_list.at(i));
		if( ((style.argument() == FontInfo::style_icons) &&
				 (info.style() == FontInfo::style_icons))
				||
				((style.argument() != FontInfo::style_icons) &&
				 (info.style() != FontInfo::style_icons)) ){

			if( font_name.is_empty() || (font_name == info.name()) ){

				if( (info.point_size() == closest_point_size) && (info.style() == closest_style) ){
					if( info.font() == nullptr ){
						info.set_font(new FileFont(info.path()));
						info.font()->set_space_size( info.font()->get_height() / 4);
					}
					return &info;
				}
			}
		}
	}

	return nullptr;
}



