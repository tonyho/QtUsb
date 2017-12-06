#include "usbexample.h"

#ifdef interface
#undef interface
#endif

UsbExample::UsbExample(QObject *parent) : QObject(parent) {
  this->setupDevice();

//  QByteArray send, recv;
  QByteArray send;
  //QByteArray sendLongData = QByteArray::fromHex("1cff2a0011000102030405060708090a0b0c0d0e0f1011121314ff02030405060708090a0b0c0d0e0f10111213dd");
  QByteArray sendLongData = QByteArray::fromHex("1cff160011000102030405060708090a0b0c0d0e0f1011121314");
  unsigned long count = 0;

  //send.append((char)0xAB);
  //0a ff 01 00 01
  send.append((char)0x0a);
  send.append((char)0xff);
  send.append((char)0x01);
  send.append((char)0x00);
  send.append((char)0x01);

  if (this->openDevice()) {
    qDebug("Device open!");
    qDebug() << "Send Data: " << send.toHex();
    this->write(&send);
    //qDebug("Read Data");
    //this->read(&recv);
  }
  while (1) {
    //qDebug("Before Sleep");
     QByteArray recv;
    //sleep(1);
     count ++;
     if(count % 20 == 1){
         qDebug() << "SendData: count = " << count << " " <<sendLongData.toHex();
         this->write(&sendLongData);
     }

    this->read(&recv);
    //qDebug("Read data: " + recv.toStdString());
    if(recv.size() != 0 || !recv.isEmpty()) {
            qDebug() << "Rec: " << recv.toHex();
            //qDebug() << "get data";
    } else {
       //qDebug() << "Empty";
    }
  }
}

UsbExample::~UsbExample() { delete mUsbDev; }

void UsbExample::setupDevice() {
  /* There are 2 ways of identifying devices depending on the platform.
   * You can use both methods, only one will be taken into account.
   */

  mUsbDev = new QUsbDevice();
  //mUsbDev->setDebug(true);

  //
  mFilter.vid = 0x248a;
  mFilter.pid = 0x5320;

  //
  mConfig.alternate = 0;
  mConfig.config = 1;
  mConfig.interface = 0;
  //mConfig.readEp = 0x05; // 88 IN, 05 OUT
  //mConfig.writeEp = 0x88;

  mConfig.readEp = 0x88; // 88 IN, 05 OUT
  mConfig.writeEp = 0x05;
}

bool UsbExample::openDevice() {
  qDebug("Opening");

  QtUsb::DeviceStatus ds;
  ds = mUsbManager.openDevice(mUsbDev, mFilter, mConfig);
  //ds = mUsbManager.openDevice(mUsbDev, mFilter, NULL);

  if (ds == QtUsb::deviceOK) {
    // Device is open
      qDebug("Open success");
    return true;
  } else {
      qDebug("Open Failed!!");
  }
  return false;
}

bool UsbExample::closeDevice() {
  qDebug("Closing");
  mUsbManager.closeDevice(mUsbDev);
  return false;
}

void UsbExample::read(QByteArray *buf) { mUsbDev->read(buf, 100); }

void UsbExample::write(QByteArray *buf) { mUsbDev->write(buf, buf->size()); }
