/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "common_values.h"

namespace APP {

namespace VERSION {

const int MAJOR = 1;
const int MINOR = 1;
const int MICRO = 1;

}  // ns APP::VERSION

namespace VALUE {

const QString NAME = "Ncalc";
const QString VERSION = QString("%1.%2.%3")
    .arg(QString::number(VERSION::MAJOR))
    .arg(QString::number(VERSION::MINOR))
    .arg(QString::number(VERSION::MICRO));
const QString AUTHORS = "N.T.Works";
const QString DESCRIPTION = ""
    "Ncalc is a simple calculator. as a feature, hexadecimal display and logical arithmetic.";
const QString LICENSE = "GNU GENERAL PUBLIC LICENSE Version 3";
const QString COPYRIGHT = "Copyright (c) 2018";

}  // ns APP::VALUE

}  // ns APP

namespace NCALC {

namespace VALUE {

}  // ns NCALC::VALUE

}  // ns NCALC
