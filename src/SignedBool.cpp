#include "SignedBool.h"

using namespace E42;

SignedBool::SignedBool()
{
    value = pos_false;
    rollover_count = 0;
}

SignedBool::SignedBool(bool sign, bool state)
{
    value = sign ? ((state) ? pos_true : pos_false) : ((state) ? neg_true : neg_false);
    rollover_count = 0;
}


SignedBool::SignedBool(const SignedBool& other)
{
    value = other.value;
    rollover_count = other.value;
}

SignedBool::SignedBool(const State& state)
{
    value = state;
    rollover_count = 0;
}

SignedBool::SignedBool(const State& state, int& count)
{
    value = state;
    rollover_count = count;
}

SignedBool SignedBool::operator&&(const SignedBool& other) const
{
    State lhs = value;
    State rhs = other.value;

    if((lhs == !rhs) || ((lhs == neg_false || rhs == neg_false)))
    {
        return SignedBool(neg_false);
    }
    else if(((lhs == rhs) && (lhs == neg_true)) + ((rhs == neg_true && lhs == pos_true) || (rhs == pos_true && lhs == neg_true)))
    {
        return SignedBool(neg_true);
    }
    else if((lhs == pos_false) || (rhs == pos_false))
    {
        return SignedBool(pos_false);
    }
    else if((lhs == pos_true) && (rhs == pos_true))
    {
        return SignedBool(pos_true);
    }
}

int SignedBool::read_count() const
{
    return rollover_count;
}

void SignedBool::inc_count()
{
    rollover_count++;
}

void SignedBool::dec_count()
{
    rollover_count--;
}

SignedBool SignedBool::operator||(const SignedBool& other) const
{
    State lhs = value;
    State rhs = other.value;

    if((lhs == neg_false) && (rhs == neg_false))
    {
        return SignedBool(neg_false);
    }
    else if((lhs == neg_true) || (rhs == neg_true))
    {
        return SignedBool(neg_true);
    }
    else if(((lhs == rhs) && (lhs == neg_true)) + ((rhs == neg_true && lhs == pos_true) || (rhs == pos_true && lhs == pos_true)))
    {
        return SignedBool(pos_false);
    }
    else if((lhs == !rhs) || ((lhs == pos_true || rhs == pos_true)))
    {
        return SignedBool(pos_true);
    }

    //i understand there are ways to simplify these further, these are the basic K-Mappings I did at 6AM from the logic statemetns I made at 4AM, it is now 9AM
}

SignedBool SignedBool::operator!() const
{
    switch(value)
    {
        case pos_true:
                return SignedBool(neg_false);
            break;
        case pos_false:
                return SignedBool(neg_true);
            break;
        case neg_true:
                return SignedBool(pos_false);
            break;
        case neg_false:
                return SignedBool(pos_true);
            break;
    }
}




SignedBool::operator bool() const
{
    switch(value)
    {
        case pos_true:
            return true;
            break;
        default:
            return false;
            break;
    }
}

SignedBool operator+(const SignedBool& lhs, const SignedBool& rhs)
{
    int flag = 0;
    if(lhs == SignedBool::pos_false || lhs == SignedBool::pos_true || rhs == SignedBool::pos_false || rhs == SignedBool::pos_true){flag ++;}

    if(
        ((lhs == SignedBool::neg_false ) && (rhs == SignedBool::neg_true)) ||
        ((lhs == SignedBool::pos_false ) && (rhs == SignedBool::pos_false)) ||
        ((lhs == SignedBool::pos_true ) && (rhs == SignedBool::neg_true))
    )
    {
        return SignedBool(SignedBool::neg_false, lhs.read_count() + rhs.read_count() + flag);
    }
    else if(
        ((lhs == SignedBool::neg_false) && (rhs == SignedBool::neg_true)) ||
        ((lhs == SignedBool::neg_true) && (rhs == SignedBool::neg_false)) ||
        ((lhs == SignedBool::pos_false) && (rhs == SignedBool::pos_true)) ||
        ((lhs == SignedBool::pos_true) && (rhs == SignedBool::pos_false))
    )
    {
        return SignedBool(SignedBool::neg_true,lhs.read_count() + rhs.read_count() + flag);
    }
    else if(
        ((lhs == SignedBool::neg_false) && (rhs == SignedBool::pos_false)) ||
        ((lhs == SignedBool::pos_false) && (rhs == SignedBool::neg_false)) ||
        ((lhs == rhs) && (lhs == SignedBool::pos_true) || (rhs == SignedBool::neg_true)) ||
        ((lhs == SignedBool::neg_true) && (rhs == SignedBool::pos_true))
    )
    {
        return SignedBool(SignedBool::pos_false, lhs.read_count() + rhs.read_count() + flag);
    }
    else if(
        (lhs == !rhs)
    )
    {
        return SignedBool(SignedBool::pos_true,lhs.read_count() + rhs.read_count() + flag);
    }
}

SignedBool operator-(const SignedBool& lhs, const SignedBool& rhs)
{
    int flag = 0;
    if(rhs > lhs){flag++;}

    if(
        ((lhs == SignedBool::neg_false || lhs == SignedBool::pos_false) && (rhs == SignedBool::neg_true || rhs == SignedBool::pos_false)) ||
        ((lhs == SignedBool::neg_false ) && (rhs != SignedBool::neg_true)) ||
        ((lhs == rhs) && (rhs == SignedBool::pos_true))
    )
    {
        if(lhs == SignedBool::neg_false && lhs == !rhs)
        {
            flag++;
        }

        return SignedBool(SignedBool::neg_false,lhs.read_count() - rhs.read_count() - flag);
    }
    else if(
        ((lhs == SignedBool::neg_false) && (rhs == SignedBool::neg_true)) ||
        ((lhs == SignedBool::neg_true) && (rhs == SignedBool::neg_false)) ||
        ((lhs == SignedBool::pos_true) && (rhs == SignedBool::pos_false))
    )
    {
        return SignedBool(SignedBool::neg_true,lhs.read_count() - rhs.read_count() - flag);
    }
    else if(
        ((lhs == SignedBool::pos_false) && (rhs == SignedBool::neg_false)) ||
        ((lhs == SignedBool::pos_true) && (rhs == SignedBool::neg_true)) ||
        ((lhs == SignedBool::neg_true) && (rhs == SignedBool::pos_true))
    )
    {
        return SignedBool(SignedBool::pos_false,lhs.read_count() - rhs.read_count() - flag);
    }
    else if(
        ((lhs == SignedBool::pos_true) && (rhs == SignedBool::neg_false)) ||
        ((lhs == SignedBool::pos_false) && (rhs == SignedBool::pos_true))
    )
    {
        return SignedBool(SignedBool::pos_true,lhs.read_count() - rhs.read_count() - flag);
    }
}

SignedBool& SignedBool::operator=(const State& newState)
{
    value = newState;
    return *this;
}

SignedBool& SignedBool::operator=(const SignedBool& other)
{
    value = other.value;
    rollover_count = other.rollover_count;
    return *this;
}


bool SignedBool::operator==(const SignedBool& other) const
{
    return (other.value == value);
}

bool SignedBool::operator<(const SignedBool& other) const
{
    if(other.value < value){return true;}
    return false;
}

bool SignedBool::operator>(const SignedBool& other) const
{
    if(other.value > value){return true;}
    return false;
}

bool SignedBool::operator<=(const SignedBool& other) const
{
    if(other < value || other == value){return true;}
    return false;
}

bool SignedBool::operator>=(const SignedBool& other) const
{
    if(other > value || other == value){return true;}
    return false;
}

bool SignedBool::operator!=(const SignedBool& other) const
{
    if(other.value != value){return true;}
    return false;
}



bool SignedBool::operator==(const State& other) const
{
    return (other == value);
}

bool SignedBool::operator<(const State& other) const
{
    if(other < value){return true;}
    return false;
}

bool SignedBool::operator>(const State& other) const
{
    if(other > value){return true;}
    return false;
}

bool SignedBool::operator<=(const State& other) const
{
    if(other < value || other == value){return true;}
    return false;
}

bool SignedBool::operator>=(const State& other) const
{
    if(other > value || other == value){return true;}
    return false;
}

bool SignedBool::operator!=(const State& other) const
{
    if(other != value){return true;}
    return false;
}

//Last Modified: 12:50PM 14July2023
