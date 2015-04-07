/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Arduino.h"
#include "UARTClass.h"
#include <Project.h>

// Constructors ////////////////////////////////////////////////////////////////

UARTClass::UARTClass(bool isUSB, SerialHelperFuncs *helpers)
{
  _isUSB = isUSB;
  _myHelpers = helpers;
  _portIsActive = false;
  _appBuffSize = 0;
  _appBuffIndex = 0;
  _timeout = 10;
  if (_isUSB)
  {
    _portIsActive = true;
  }
}

// Public Methods //////////////////////////////////////////////////////////////

void UARTClass::begin(const uint32_t dwBaudRate)
{
  if (!_isUSB)
  {
    _portIsActive = _myHelpers->portEnable();
    _myHelpers->clockAdjust(24000000/(8*dwBaudRate));
  }
}

void UARTClass::end( void )
{
  if (!_isUSB)
  {
    _portIsActive = false;
    _myHelpers->portDisable();
  }
}


int UARTClass::available( void )
{
  if (_appBuffSize > 0)
  {
    return _appBuffSize;
  }
  return _myHelpers->bufferSize();
}

int UARTClass::peek( void )
{
  if (_appBuffSize == 0)
  {
    _appBuffSize = _myHelpers->bufferSize();
    if (_appBuffSize == 0)
    {
      return -1;
    }
    _myHelpers->getData(_appBuffer, _appBuffSize);
    _appBuffIndex = 0;
  }
  return _appBuffer[_appBuffIndex];
}

int UARTClass::read( void )
{
  if (_appBuffSize == 0) 
  {
    _appBuffSize = _myHelpers->bufferSize();
    if(_appBuffSize == 0)
    {
      return -1;
    }
    _myHelpers->getData(_appBuffer, _appBuffSize);
    _appBuffIndex = 0;
  }
  --_appBuffSize;
  return _appBuffer[_appBuffIndex++];
}

void UARTClass::flush( void )
{
  if (!_isUSB)
  {
    _myHelpers->blockForWriteComplete();
  }
}

size_t UARTClass::write( const uint8_t uc_data )
{
  if (!_portIsActive) return 0;
  while (_myHelpers->blockForReadyToWrite() == 0)
  { /* wait for the port to be ready */  }
  _myHelpers->sendData(const_cast<uint8_t*>(&uc_data), 1);
  return 1;
}

