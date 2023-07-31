#ifndef E42_SignedBool_H
#define E42_SignedBool_H

/*
SignedBool Written
---E42 Technologies---
Authors:
[EL,EC,LY,OP]

Created: 13JULY2023 2:18:06 PM UTC

Authors Message:
The applications are limitless! -EL

Designed to do what booleans can't. -EC

Everything is either a boolean or not a boolean, and true or false. -LY
*/


namespace E42
{
    class SignedBool
    {
        public:

            static enum State{pos_true, pos_false, neg_true, neg_false};

            SignedBool();
            SignedBool(bool=false, bool=false);
            SignedBool(const SignedBool&);
            SignedBool(const State&);
            SignedBool(const State&, int&);

            SignedBool operator&&(const SignedBool&) const;
            SignedBool operator||(const SignedBool&) const;
            SignedBool operator!() const;

            SignedBool& operator=(const SignedBool&);
            SignedBool& operator=(const State&);

            bool operator==(const SignedBool&) const;
            bool operator==(const State&) const;

            bool operator<(const SignedBool&) const;
            bool operator<(const State&) const;

            bool operator>(const SignedBool&) const;
            bool operator>(const State&) const;

            bool operator<=(const SignedBool&) const;
            bool operator<=(const State&) const;

            bool operator>=(const SignedBool&) const;
            bool operator>=(const State&) const;

            bool operator!=(const SignedBool&) const;
            bool operator!=(const State&) const;


            int read_count() const;

            void dec_count();
            void inc_count();


            explicit operator bool()const;

            friend SignedBool operator+(const SignedBool&, const SignedBool&);
            friend SignedBool operator-(const SignedBool&, const SignedBool&);
    private:

            State value;
            int rollover_count;
    };
}

//Last Modified: 12:50PM 14July2023

#endif
