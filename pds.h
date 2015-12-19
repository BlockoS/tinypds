#ifndef PDS_H
#define PDS_H

#include <stdint.h>

/**
 * Status values.
 */
enum PDS_STATUS
{
    PDS_OK = 0,
    PDS_INVALID_VALUE,
    PDS_INVALID_RANGE
};
/**
 * Units.
 */
enum PDS_UNITS
{
    PDS_UNIT_UNKNOWN = 0,   /**< Unknown unit */
    PDS_UNIT_A,             /**< Ampere (A) */
    PDS_UNIT_A_m,           /**< Ampere per meter (A/m) */
    PDS_UNIT_A_m2,          /**< Ampere per square meter (A/m^2) */
    PDS_UNIT_B,             /**< Bytes (B) */
    PDS_UNIT_Bq,            /**< Becquerel (Bq) */
    PDS_UNIT_C,             /**< Coulomb (C) */
    PDS_UNIT_C_Kg,          /**< Coulomb per kilogram (C/kg) */
    PDS_UNIT_C_m2,          /**< Coulomb per square meter (C/m^2) */   
    PDS_UNIT_C_m3,          /**< Coulomb per cubic meter (C/m^3) */   
    PDS_UNIT_F,             /**< Farad (F) */
    PDS_UNIT_F_m,           /**< Farad per meter (F/m) */
    PDS_UNIT_GHz,           /**< Gigahertz (GHz) */
    PDS_UNIT_Gy,            /**< Gray (Gy) */
    PDS_UNIT_Gy_s,          /**< Gray per second (Gy/s) */
    PDS_UNIT_H,             /**< Henry (H) */
    PDS_UNIT_H_m,           /**< Henry per meter (H/m) */
    PDS_UNIT_Hz,            /**< Hertz (Hz) */
    PDS_UNIT_J,             /**< Joule (J) */
    PDS_UNIT_J_KgK,         /**< Joule per kilogram kelvin (J/(kg.K)) */
    PDS_UNIT_J_m2_s,        /**< Joule per square meter per second (J/m^2/s) */
    PDS_UNIT_J_molK,        /**< Joule per mole kelvin (J/(mol.K)) */
    PDS_UNIT_J_K,           /**< Joule per kelvin (J/K) */
    PDS_UNIT_J_T,           /**< Joule per tesla (J/T) */
    PDS_UNIT_J_Kg,          /**< Joule per kilogram (J/Kg) */
    PDS_UNIT_J_m3,          /**< Joule per cubic meter (J/m^3) */
    PDS_UNIT_J_mol,         /**< Joule per mole (J/mol) */
    PDS_UNIT_K,             /**< Kelvin (K) */
    PDS_UNIT_MB,            /**< Mega Bytes (MB) */
    PDS_UNIT_MHz,           /**< Megahertz (MHz) */
    PDS_UNIT_N,             /**< Newton (N) */
    PDS_UNIT_Nm,            /**< Newton meter (N.m) */
    PDS_UNIT_Nms,           /**< Newton meter second (N.m.s) */
    PDS_UNIT_NONE,          /**< No unit of measurement defined */
    PDS_UNIT_N_m,           /**< Newton per meter (N/m) */
    PDS_UNIT_N_m2,          /**< Newton per square meter (N/m^2) */
    PDS_UNIT_Pa,            /**< Pascal (Pa) */
    PDS_UNIT_Pas,           /**< Pascal second (Pa.s) */
    PDS_UNIT_S,             /**< Siemens (S) */
    PDS_UNIT_Sv,            /**< Sievert (Sv) */
    PDS_UNIT_T,             /**< Tesla (T) */
    PDS_UNIT_V,             /**< Volt (V) */
    PDS_UNIT_V_m,           /**< Volt per meter (V/m) */
    PDS_UNIT_W,             /**< Watt (W) */
    PDS_UNIT_W_m2_sr,       /**< Watt per square meter per steradian (W/m^2/sr) */
    PDS_UNIT_W_m2_sr_nm,    /**< Watt per square meter per steradian per nanometer (W/m^2/sr/nm) */
    PDS_UNIT_W_m2_sr_um,    /**< Watt per square meter per steradian per micron (W/m^2/sr/um) */
    PDS_UNIT_W_mK,          /**< Watt per meter kelvin (W/(m.K)) */
    PDS_UNIT_W_m2,          /**< Watt per square meter (W/m^2) */
    PDS_UNIT_W_sr,          /**< Watt per steradian (W/sr) */
    PDS_UNIT_Wb,            /**< Weber (Wb) */
    PDS_UNIT_arcsec_px,     /**< Arcsecond per pixel (arcsec/pixel) */
    PDS_UNIT_arcsec,        /**< Arcsecond (arcsec) */
    PDS_UNIT_b_px,          /**< Bits per pixel (b/pixel) */
    PDS_UNIT_b_s,           /**< Bits per second (b/s) */
    PDS_UNIT_bar,           /**< Bar (bar) */
    PDS_UNIT_cd,            /**< Candela (cd) */
    PDS_UNIT_cd_m2,         /**< Candela per square meter (cd/m^2) */
    PDS_UNIT_count_min,     /**< Counts per minute (counts/min) */
    PDS_UNIT_d,             /**< Day (d) */
    PDS_UNIT_dB,            /**< Decibel (dB) */
    PDS_UNIT_deg,           /**< Degree (deg) */
    PDS_UNIT_deg_day,       /**< Degree per day (deg/day) */
    PDS_UNIT_deg_s,         /**< Degree per second (deg/s) */
    PDS_UNIT_c,             /**< Degree Celsius (c) */
    PDS_UNIT_fps,           /**< Frame per second (fps) */
    PDS_UNIT_g,             /**< Gram (g) */
    PDS_UNIT_g_cm3,         /**< Gram per cubic centimeter (g/cm^3) */
    PDS_UNIT_h,             /**< Hour (h) */
    PDS_UNIT_kHz,           /**< Kilohertz (kHz) */
    PDS_UNIT_kb_s,          /**< Kilobits per second (kb/s) */
    PDS_UNIT_kg,            /**< Kilogram (kg) */
    PDS_UNIT_kg_m3,         /**< Kilogram per cubic meter (kg/m^3) */
    PDS_UNIT_km,            /**< Kilometer (km) */
    PDS_UNIT__km,           /**< Reciprocal kilometer (km^-1) */
    PDS_UNIT_km2,           /**< Kilometer squared (km^2) */
    PDS_UNIT_km_px,         /**< Kilometer per pixel (km/px) */
    PDS_UNIT_km_s,          /**< Kilometer per second (km/s) */
    PDS_UNIT_lm,            /**< Lumen (lm) */
    PDS_UNIT_lx,            /**< Lux (lx) */
    PDS_UNIT_m,             /**< Meter (m) */
    PDS_UNIT__m,            /**< Reciprocal meter (m^-1) */
    PDS_UNIT_m2,            /**< Square meter (m^2) */
    PDS_UNIT_m2_s,          /**< Square meter per second (m^2/s) */
    PDS_UNIT_m3,            /**< Cubic meter (m^3) */
    PDS_UNIT_m3_kg,         /**< Cubic meter per kilogram (m^3/kg) */
    PDS_UNIT_m_px,          /**< Meters per pixel (m/px) */
    PDS_UNIT_m_s,           /**< Meters per second (m/s) */
    PDS_UNIT_m_s2,          /**< Meters per second squared (m/s^2) */
    PDS_UNIT_mA,            /**< Milliampere (mA) */
    PDS_UNIT_mag,           /**< Magnitude (mag) */
    PDS_UNIT_um,            /**< Micron (um) */
    PDS_UNIT_min,           /**< Minute (min) */
    PDS_UNIT_mm,            /**< Millimiter (mm) */
    PDS_UNIT_mm_s,          /**< Millimiter per second (mm/s) */
    PDS_UNIT_mol,           /**< Mole (mol) */
    PDS_UNIT_mol_m3,        /**< Mole per cubic meter (mol/m^3) */
    PDS_UNIT_mrad,          /**< Milliradians (mrad) */
    PDS_UNIT_ms,            /**< Millisecond (ms) */
    PDS_UNIT_nT,            /**< Nanotesla (nT) */
    PDS_UNIT_nm,            /**< Nanometer (nm) */
    PDS_UNIT_ohm,           /**< Ohm (ohm) */
    PDS_UNIT_px_line,       /**< Pixels per line (px/line) */
    PDS_UNIT_px_deg,        /**< Pixels per degree (px/deg) */
    PDS_UNIT_px,            /**< Pixel (px) */
    PDS_UNIT_rad,           /**< Radian (rad) */
    PDS_UNIT_rad_s,         /**< Radian per second (rad/s) */
    PDS_UNIT_rad_s2,        /**< Radian per second squared (rad/s^2) */
    PDS_UNIT_s,             /**< Second (s) */
    PDS_UNIT_sr,            /**< Steradian (sr) */
    PDS_UNIT_uW,            /**< Microwatts (uW) */
    PDS_UNIT_us,            /**< Microsecond (us) */
    PDS_UNIT_usd,           /**< United states dollars (us_dollar) */
};
/**
 * Parse an integer number.
 * The number can be expressed in decimal or based notation.
 * A based integer is represented by a decimal number representing the base
 * followed by a number expressed in the specified base enclosed between '#'
 * characters.
 * @param [in] first First character of the input string.
 * @param [in] last Last character of the input string.
 * @param [out] end If not null stores the pointer to the first invalid
 *                  character of the input string. 
 * @param [in][out] status Status variable set to PDS_OK if an integer was
 *                  successfully parsed. If an error occured it is set to 
 *                  PDS_INVALID_VALUE or PDS_INVALID_RANGE.
 * @return Parsed integer. In case of overflow INT32_MAX or (INT32_MIN for a
 * negative value) is returned, and the status variable is set to
 * PDS_INVALID_RANGE. If no conversion was performed, 0 is returned and the
 * status variable is set to PDS_INVALUD_VALUE.
 * @return Parsed integer. In case of overflow INT32_MAX or (INT32_MIN for a
 * negative value) is returned, and the status variable is set to
 * PDS_INVALID_RANGE. If no conversion was performed, 0 is returned and the
 * status variable is set to PDS_INVALUD_VALUE.
 */
int32_t PDS_parse_int(const char* first, const char* last, const char** end, int *status);
/**
 * Parse a floating point value.
 * @param [in] first First character of the input string.
 * @param [in] last Last character of the input string.
 * @param [out] end If not null stores the pointer to the first invalid
 *                  character of the input string. 
 * @param [in] base Base in which the unsigned integer is encoded. The base must
 *                  be in the range 2 to 16. 
 * @param [in][out] status Status variable set to PDS_OK if an integer was
 *                  successfully parsed. If an error occured it is set to 
 *                  PDS_INVALID_VALUE or PDS_INVALID_RANGE.
 * @return Parsed floating point value. If an error occured or no conversion occured
 * 0.0 is returned.
 */
double PDS_parse_real(const char *first, const char *last, const char **end, int *status);

/**
 * [todo]
 */
int PDS_parse_unit(const char *first, const char *last, const char **end, int *status);

/* [todo]
 * struct value
 * {
 *    union { int32_t, double, };
 *    type { INTEGER, REAL }
 *    unit { enum from pdsdd.full }
 * }
 */
/* [todo] parse string */
/* [todo] parse list */
/* [todo] parse record */
/* [todo] parse date */

#endif /* PDS_H */

#ifdef PDS_IMPL

#define PDS_isspace(c) (    (' '  == (c)) \
                         || ('\f' == (c)) \
                         || ('\n' == (c)) \
                         || ('\r' == (c)) \
                         || ('\t' == (c)) \
                         || ('\v' == (c)) )

#define PDS_isdigit(c) (((c) >= '0') && ((c) <= '9'))

#define PDS_islower(c) (((c) >= 'a') && ((c) <= 'z'))

#define PDS_isupper(c) (((c) >= 'A') && ((c) <= 'Z'))

#define PDS_isalpha(c) (PDS_islower(c) || PDS_isupper(c))

/**
 * Remove leading and trailing white spaces in a string.
 * A white space is either a space (' '), form-feed ('\f'), newline ('\n'),
 * carriage return ('\r'), horizontal tab ('\t') or vertical tab ('\v').
 * @param [in]  str   Input string.
 * @param [in]  len   Input string length.
 * @param [out] first Index of the first non-space character in the string.
 * @param [out] last  Index of the last non-space character in the string.
 */
static void trim(const char* str, int len, const char** first, const char** last)
{
    int i;
    for(i=0; (i<len) && PDS_isspace(str[i]); ++i) 
    {}
    if(i<len)
    {
        *first = str+i;
        for(i=len-1; (i>=0) && PDS_isspace(str[i]); --i)
        {}
        *last = str+i;
    }
    else
    {
        *first = *last = 0;
    }
}

/**
 * Parse an integer in the specified base.
 * The number base must be in the range 2 to 16. Letters 'A' to 'F' in lower or
 * upper case are used for digits greater than 10. 
 * @param [in] first First character of the input string.
 * @param [in] last Last character of the input string.
 * @param [out] end If not null stores the pointer to the first invalid
 *                  character of the input string. 
 * @param [in] base Base in which the unsigned integer is encoded. The base must
 *                  be in the range 2 to 16. 
 * @param [in][out] status Status variable set to PDS_OK if an integer was
 *                  successfully parsed. If an error occured it is set to 
 *                  PDS_INVALID_VALUE or PDS_INVALID_RANGE.
 * @return Parsed integer. In case of overflow INT32_MAX or (INT32_MIN for a
 * negative value) is returned, and the status variable is set to
 * PDS_INVALID_RANGE. If no conversion was performed, 0 is returned and the
 * status variable is set to PDS_INVALUD_VALUE.
 */
static int32_t parse_int(const char *first, const char *last, const char **end, int base, int *status)
{
    const char *ptr;
    int64_t current;
    int64_t accumulator;
    int64_t cutoff;
    int64_t cutlimit;
    int any;
    int neg;
    /* Sanity check. */
    if(PDS_OK != *status)
    {
        return 0;
    }
    if((base<2) || (base>16))
    {
        *status = PDS_INVALID_VALUE;
        return INT32_MAX;
    }
    /* Trim spaces. */
    for(ptr=first; (ptr<=last) && PDS_isspace(*ptr); ++ptr)
    {}
    /* Check for possible sign. */
    if('-' == *ptr)
    {
        neg = 1;
        ++ptr;
    }
    else 
    {
        neg = 0;
        if('+' == *ptr)
        {
            ++ptr;
        }
    }
    /*
     * Compute the cutoff value between legal and illegal number of the
     * accumulator prior to adding the last read digit. If the accumulator is
     * egal to this cutoff value, the last read digit must not produce an
     * invalid result.
     * In other words, the cuttoff value is MAX_INT/base and the limit is
     * MAX_INT%base. 
     */
    cutoff   = neg ? -(int64_t)INT32_MIN : INT32_MAX; 
    cutlimit = cutoff % base;
    cutoff  /= base; 
    /* Parse integer. */
    for(accumulator=0, any=0; ptr<=last; ++ptr)
    {
        if(PDS_isdigit(*ptr))
        {
            current = *ptr - '0';
        }
        else if(PDS_isalpha(*ptr))
        {
            current = *ptr - (PDS_islower(*ptr) ? ('a'-10) : ('A'-10));
        }
        else
        {
            break;
        }
        if(current >= base)
        {
            break;
        }
        if(any >= 0)
        {
            if(   (accumulator >  cutoff)
               || ((accumulator == cutoff) && (current > cutlimit)) )
            {
                any = -1;
            }
            else
            {
                any = 1;
                accumulator *= base;
                accumulator += current;
            }
        }
    }
    if(0 != end)
    {
        *end = any ? ptr : first;
    }
    if(any < 0)
    {
        *status = PDS_INVALID_RANGE;
        return neg ? INT32_MIN : INT32_MAX; 
    }
    else
    {
        *status = any ? PDS_OK : PDS_INVALID_VALUE;
        return (int32_t)(neg ? -accumulator : accumulator);
    }
}

/*
 * Parse an integer number.
 */
int32_t PDS_parse_int(const char* first, const char* last, const char** end, int *status)
{
    const char *ptr;
    int32_t value;
    /* Sanity check. */
    if(PDS_OK != *status)
    {
        return INT32_MAX;
    }
    if(0 == end)
    {
        *status = PDS_INVALID_VALUE;
        return INT32_MAX;   
    }
    *end = first;
    /* Parse value. */
    value = parse_int(first, last, &ptr, 10, status);
    if(PDS_OK != *status)
    {
        return INT32_MAX;
    }
    /* 
     * If the character following the current number is a '#', this may mean
     * that the number is in fact expressed in based notation.
     */
    if((ptr >= last) || (*ptr != '#'))
    {
        *end = ptr;
        return value;
    }
    if((value < 2) || (value > 16))
    {
        *status = PDS_INVALID_RANGE;
        return INT32_MAX;
    }
    ++ptr;
    value = parse_int(ptr, last, end, value, status);
    if(PDS_OK == *status)
    {
        /* Check that the number is followed by a closing '#'. */
        if('#' != **end)
        {
            *status = PDS_INVALID_VALUE;
            value = INT32_MAX;
        }
        else
        {
            ++*end;
        }
    }
    return value;
}

/*
 * Parse a floating point value.
 */
double PDS_parse_real(const char *first, const char *last, const char **end, int *status) 
{
    double value;
    int neg = 0;
    int div;
    int exponent;
    int32_t integer;
    int32_t decimal;
    const char *ptr;
    *end = first;
    /* Integer part (can be negative). */   
    integer = parse_int(first, last, &ptr, 10, status);
    /* The integer part can be empty (ex: .03). */
    if(ptr == first)
    {
        integer = 0;
        *status = PDS_OK;
        if('-' == *ptr)
        {
            neg = 1;
            ++ptr;
        }
        else if('+' == *ptr)
        {
            ++ptr;
        }
    }
    else if(integer < 0)
    {
        neg = 1;
    }
    if(PDS_OK != *status)
    {
        return 0.0;
    }
    /* Check for decimal part. */
    if('.' != *ptr)
    {
        *end = ptr;
        return (PDS_OK == *status) ? (double)integer : 0.0;
    }

    first = ptr+1;
    decimal = parse_int(first, last, &ptr, 10, status);
    if((ptr == first) && (0 == decimal))
    {
        *status = PDS_OK;
    }
    if(PDS_OK != *status)
    {
        return 0.0;
    }
    if(decimal < 0)
    {
        *status = PDS_INVALID_VALUE;
        return 0.0;
    }
    for(div=1; first<ptr; ++first, div*=10)
    {}
    value = integer + ((neg?-decimal:decimal) / (double)div);   
    
    /* And lastly the exponent. */
    exponent = 1;
    if(('e' == *ptr) || ('E' == *ptr))
    {
        int32_t i;
        int32_t n;
        first = ptr+1;
        n = parse_int(first, last, &ptr, 10, status);
        if(PDS_OK != *status)
        {
            *status = PDS_INVALID_VALUE;
            return 0.0;
        }
        if(n < 0)
        {
            for(i=0, div=1; i>n; --i, div*=10)
            {}
            value /= (double)div;
        }
        else
        {
            for(i=0, exponent=1; i<n; ++i, exponent*=10)
            {}
            value *= (double)exponent;
        }
    }
    *end = ptr;
    return value;   
}

/*
 * Parse 
 */
int PDS_parse_unit(const char *first, const char *last, const char **end, int *status)
{
    struct 
    {
        const char *str;
        int         unit;
    } dict[] =
    {
        {"A",         PDS_UNIT_A      }, {"A/m",       PDS_UNIT_A_m    },
        {"A/m**2",    PDS_UNIT_A_m2   }, {"B",         PDS_UNIT_B      },
        {"Bq",        PDS_UNIT_Bq     }, {"C",         PDS_UNIT_C      },
        {"C/kg",      PDS_UNIT_C_Kg   }, {"C/m**2",    PDS_UNIT_C_m2   },
        {"C/m**3",    PDS_UNIT_C_m3   }, {"F",         PDS_UNIT_F      },
        {"F/m",       PDS_UNIT_F_m    }, {"GHz",       PDS_UNIT_GHz    },
        {"Gy",        PDS_UNIT_Gy     }, {"Gy/s",      PDS_UNIT_Gy_s   },
        {"H",         PDS_UNIT_H      }, {"H/m",       PDS_UNIT_H_m    },
        {"Hz",        PDS_UNIT_Hz     }, {"J",         PDS_UNIT_J      },
        {"J/(kg.K)",  PDS_UNIT_J_KgK  }, {"J/(m**2)/s",PDS_UNIT_J_m2_s },
        {"J/(mol.K)", PDS_UNIT_J_molK }, {"J/K",       PDS_UNIT_J_K    }, 
        {"J/T",       PDS_UNIT_J_T    }, {"J/kg",      PDS_UNIT_J_Kg   },
        {"J/m**3",    PDS_UNIT_J_m3   }, {"J/mol",     PDS_UNIT_J_mol  },
        {"K",         PDS_UNIT_K      }, {"MB",        PDS_UNIT_MB     },
        {"MHz",       PDS_UNIT_MHz    }, {"N",         PDS_UNIT_N      },
        {"N.m",       PDS_UNIT_Nm     }, {"N.m.s",     PDS_UNIT_Nms    },
        {"N/A",       PDS_UNIT_NONE   }, {"N/m",       PDS_UNIT_N_m    },
        {"N/m**2",    PDS_UNIT_N_m2   }, {"Pa",        PDS_UNIT_Pa     },
        {"Pa.s",      PDS_UNIT_Pas    }, {"S",         PDS_UNIT_S      },
        {"Sv",        PDS_UNIT_Sv     }, {"T",         PDS_UNIT_T      },
        {"V",         PDS_UNIT_V      }, {"V/m",       PDS_UNIT_V_m    },
        {"W",         PDS_UNIT_W      },
        {"W.m**-2.sr**-1",        PDS_UNIT_W_m2_sr    },
        {"W.m**-2.sr**-1.nm**-1", PDS_UNIT_W_m2_sr_nm },
        {"W/(m**2*sr*micron)",    PDS_UNIT_W_m2_sr_um },
        {"W/(m.K)",   PDS_UNIT_W_mK   }, {"W/m**2",    PDS_UNIT_W_m2   },
        {"W/sr",      PDS_UNIT_W_sr   }, {"Wb",        PDS_UNIT_Wb     },
        {"arcsec/pixel", PDS_UNIT_arcsec_px }, 
        {"arcsecond", PDS_UNIT_arcsec }, {"b/pixel",   PDS_UNIT_b_px   },
        {"b/s",       PDS_UNIT_b_s    }, {"bar",       PDS_UNIT_bar    },
        {"cd",        PDS_UNIT_cd     }, {"cd/m**2",   PDS_UNIT_cd_m2  },
        {"counts/minute", PDS_UNIT_count_min },
        {"d",         PDS_UNIT_d      }, {"dB",        PDS_UNIT_dB     },
        {"deg",       PDS_UNIT_deg    }, {"deg/day",   PDS_UNIT_deg_day},
        {"deg/s",     PDS_UNIT_deg_s  }, {"degC",      PDS_UNIT_c      },
        {"fps",       PDS_UNIT_fps    }, {"g",         PDS_UNIT_g      },
        {"g/cm**3",   PDS_UNIT_g_cm3  }, {"h",         PDS_UNIT_h      },
        {"kHz",       PDS_UNIT_kHz    }, {"kb/s",      PDS_UNIT_kb_s   },
        {"kg",        PDS_UNIT_kg     }, {"kg/m**3",   PDS_UNIT_kg_m3  },
        {"km",        PDS_UNIT_km     }, {"km**-1",    PDS_UNIT__km    },
        {"km**2",     PDS_UNIT_km2    }, {"km/pix",    PDS_UNIT_km_px  },
        {"km/pixel",  PDS_UNIT_km_px  }, {"km/s",      PDS_UNIT_km_s   },
        {"lm",        PDS_UNIT_lm     },
        {"local day/24", PDS_UNIT_UNKNOWN },
        {"lx",        PDS_UNIT_lx     }, {"m",         PDS_UNIT_m      },
        {"m**-1",     PDS_UNIT__m     }, {"m**2",      PDS_UNIT_m2     },
        {"m**2/s",    PDS_UNIT_m2_s   }, {"m**3",      PDS_UNIT_m3     },
        {"m**3/kg",   PDS_UNIT_m3_kg  }, {"m/pix",     PDS_UNIT_m_px   },
        {"m/pixel",   PDS_UNIT_m_px   }, {"m/s",       PDS_UNIT_m_s    },
        {"m/s**2",    PDS_UNIT_m_s2   }, {"mA",        PDS_UNIT_mA     },
        {"mag",       PDS_UNIT_mag    }, {"micron",    PDS_UNIT_um     },
        {"min",       PDS_UNIT_min    }, {"mm",        PDS_UNIT_mm     },
        {"mm/s",      PDS_UNIT_mm_s   }, {"mol",       PDS_UNIT_mol    },
        {"mol/m**3",  PDS_UNIT_mol_m3 }, {"mrad",      PDS_UNIT_mrad   },
        {"ms",        PDS_UNIT_ms     }, {"n/a",       PDS_UNIT_NONE   },
        {"nT",        PDS_UNIT_nT     }, {"nm",        PDS_UNIT_nm     },
        {"none",      PDS_UNIT_NONE   }, {"ohm",       PDS_UNIT_ohm    },
        {"p/line",    PDS_UNIT_px_line}, {"pix/deg",   PDS_UNIT_px_deg },
        {"pix/degree",PDS_UNIT_px_deg }, {"pixel",     PDS_UNIT_px     },
        {"pixel/deg",    PDS_UNIT_px_deg },
        {"pixel/degree", PDS_UNIT_px_deg },
        {"rad",       PDS_UNIT_rad    }, {"rad/s",     PDS_UNIT_rad_s  },
        {"rad/s**2",  PDS_UNIT_rad_s2 }, {"s",         PDS_UNIT_s      },
        {"sr",        PDS_UNIT_sr     }, {"uW",        PDS_UNIT_uW     },
        {"us",        PDS_UNIT_us     }, {"us_dollar", PDS_UNIT_usd    },
        { 0, PDS_UNIT_UNKNOWN }
    };
    int i;
    int j;
    int len;
    const char *ptr = first;
    if(('<' == *first) && ('>' == *last))
    {
        ++ptr;
        len = last-ptr;
        for(i=0; dict[i].str; i++)
        {
            for(j=0; (j<len) && dict[i].str[j] && (dict[i].str[j] == ptr[j]); j++)
            {}
            if(j==len)
            {
                *end = last+1;
                *status = PDS_OK;
                return dict[i].unit;
            }
        }
    }
    *end = first;
    *status = PDS_INVALID_VALUE;
    return PDS_UNIT_UNKNOWN;    
}

#endif /* PDS_IMPL */
