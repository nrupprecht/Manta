#include "cparse.hpp"

namespace Manta {

	bool CC_Parser::parse(const vector<Lexeme>& input) {
		lex_list = input;
		int point = 0;
		level = 0;
		bool success = check_program(point);
		cout << (point==lex_list.size() ? "Whole program parsed." : "Not all parsed.") << endl;
		return (success && point==lex_list.size());
	}

	bool CC_Parser::check_boolean(int& point) {
		bool valid = false;
		int local_point = point;

		// Debug statement.
		cout << tabs() << "Entering function [boolean]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
		++level;
		// <---

		if (lex_list[local_point++].literal=="true") valid = true;
		if (!valid && lex_list[local_point++].literal=="false") valid = true;

		// Debug statement.
		--level;

		cout << tabs() << "Exiting function [boolean] with " << (valid ? "success" : "failure") << ".\n";
		// <---
		// Did any rule succeed?
		if (valid) {
			point = local_point;
			return true;
		}
		// Else, return false.
		return false;
	}

	bool CC_Parser::check_identifier(int& point) {
		bool valid = false;
		int local_point = point;

		// Debug statement.
		cout << tabs() << "Entering function [identifier]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
		++level;
		// <---

		if (lex_list[local_point++].literal=="var") {
			bool valid_branch = true;
			// Debug statement.
			if (valid_branch) cout << tabs() << "Checking for lex type [Identifier]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
			// <---

			if (valid_branch && lex_list[local_point++].type!=LexType::Identifier) valid_branch = false;
			// If all test have been passed.
			if (valid_branch) valid = true;
			else local_point = point; // To be ready to try the next option.
		}
		else --local_point;

		// Debug statement.
		--level;

		cout << tabs() << "Exiting function [identifier] with " << (valid ? "success" : "failure") << ".\n";
		// <---
		// Did any rule succeed?
		if (valid) {
			point = local_point;
			return true;
		}
		// Else, return false.
		return false;
	}

	bool CC_Parser::check_param(int& point) {
		bool valid = false;
		int local_point = point;

		// Debug statement.
		cout << tabs() << "Entering function [param]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
		++level;
		// <---

		if (check_identifier(local_point)) valid = true;

		// Debug statement.
		--level;

		cout << tabs() << "Exiting function [param] with " << (valid ? "success" : "failure") << ".\n";
		// <---
		// Did any rule succeed?
		if (valid) {
			point = local_point;
			return true;
		}
		// Else, return false.
		return false;
	}

	bool CC_Parser::check_param_list(int& point) {
		bool valid = false;
		int local_point = point;

		// Debug statement.
		cout << tabs() << "Entering function [param_list]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
		++level;
		// <---

		if (check_param(local_point)) {
			bool valid_branch = true;
			// Debug statement.
			if (valid_branch) cout << tabs() << "Checking for production [param_list]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
			// <---

			if (valid_branch && !check_param_list(local_point)) valid_branch = false;
			// If all test have been passed.
			if (valid_branch) valid = true;
			else local_point = point; // To be ready to try the next option.
		}
		if (!valid && true) valid = true;

		// Debug statement.
		--level;

		cout << tabs() << "Exiting function [param_list] with " << (valid ? "success" : "failure") << ".\n";
		// <---
		// Did any rule succeed?
		if (valid) {
			point = local_point;
			return true;
		}
		// Else, return false.
		return false;
	}

	bool CC_Parser::check_program(int& point) {
		bool valid = false;
		int local_point = point;

		// Debug statement.
		cout << tabs() << "Entering function [program]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
		++level;
		// <---

		if (check_statement_list(local_point)) valid = true;

		// Debug statement.
		--level;

		cout << tabs() << "Exiting function [program] with " << (valid ? "success" : "failure") << ".\n";
		// <---
		// Did any rule succeed?
		if (valid) {
			point = local_point;
			return true;
		}
		// Else, return false.
		return false;
	}

	bool CC_Parser::check_statement(int& point) {
		bool valid = false;
		int local_point = point;

		// Debug statement.
		cout << tabs() << "Entering function [statement]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
		++level;
		// <---

		if (lex_list[local_point++].literal=="if") {
			bool valid_branch = true;
			// Debug statement.
			if (valid_branch) cout << tabs() << "Checking for production [boolean]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
			// <---

			if (valid_branch && !check_boolean(local_point)) valid_branch = false;
			// Debug statement.
			if (valid_branch) cout << tabs() << "Checking for literal \"then\". Next token has literal ["<< lex_list[local_point].literal << "].\n";
			// <---

			if (valid_branch && lex_list[local_point++].literal!="then") valid_branch = false;
			// Debug statement.
			if (valid_branch) cout << tabs() << "Checking for lex type [OpenBrack]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
			// <---

			if (valid_branch && lex_list[local_point++].type!=LexType::OpenBrack) valid_branch = false;
			// Debug statement.
			if (valid_branch) cout << tabs() << "Checking for lex type [CloseBrack]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
			// <---

			if (valid_branch && lex_list[local_point++].type!=LexType::CloseBrack) valid_branch = false;
			// Debug statement.
			if (valid_branch) cout << tabs() << "Checking for lex type [NewLine]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
			// <---

			if (valid_branch && lex_list[local_point++].type!=LexType::NewLine) valid_branch = false;
			// If all test have been passed.
			if (valid_branch) valid = true;
			else local_point = point; // To be ready to try the next option.
		}
		else --local_point;
		if (!valid && lex_list[local_point++].literal=="while") {
			bool valid_branch = true;
			// Debug statement.
			if (valid_branch) cout << tabs() << "Checking for production [boolean]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
			// <---

			if (valid_branch && !check_boolean(local_point)) valid_branch = false;
			// Debug statement.
			if (valid_branch) cout << tabs() << "Checking for lex type [OpenBrack]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
			// <---

			if (valid_branch && lex_list[local_point++].type!=LexType::OpenBrack) valid_branch = false;
			// Debug statement.
			if (valid_branch) cout << tabs() << "Checking for lex type [CloseBrack]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
			// <---

			if (valid_branch && lex_list[local_point++].type!=LexType::CloseBrack) valid_branch = false;
			// Debug statement.
			if (valid_branch) cout << tabs() << "Checking for lex type [NewLine]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
			// <---

			if (valid_branch && lex_list[local_point++].type!=LexType::NewLine) valid_branch = false;
			// If all test have been passed.
			if (valid_branch) valid = true;
			else local_point = point; // To be ready to try the next option.
		}
		else --local_point;

		// Debug statement.
		--level;

		cout << tabs() << "Exiting function [statement] with " << (valid ? "success" : "failure") << ".\n";
		// <---
		// Did any rule succeed?
		if (valid) {
			point = local_point;
			return true;
		}
		// Else, return false.
		return false;
	}

	bool CC_Parser::check_statement_list(int& point) {
		bool valid = false;
		int local_point = point;

		// Debug statement.
		cout << tabs() << "Entering function [statement_list]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
		++level;
		// <---

		if (check_statement(local_point)) {
			bool valid_branch = true;
			// Debug statement.
			if (valid_branch) cout << tabs() << "Checking for production [statement_list]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
			// <---

			if (valid_branch && !check_statement_list(local_point)) valid_branch = false;
			// If all test have been passed.
			if (valid_branch) valid = true;
			else local_point = point; // To be ready to try the next option.
		}
		if (!valid && check_statement(local_point)) valid = true;

		// Debug statement.
		--level;

		cout << tabs() << "Exiting function [statement_list] with " << (valid ? "success" : "failure") << ".\n";
		// <---
		// Did any rule succeed?
		if (valid) {
			point = local_point;
			return true;
		}
		// Else, return false.
		return false;
	}

}