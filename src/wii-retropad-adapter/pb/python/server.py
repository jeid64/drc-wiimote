import socket
import sys
import struct
import controller_state_pb2

import serial

def send_message(sock, message):
    """ Send a serialized message (protobuf Message interface)
        to a socket, prepended by its length packed in 4
        bytes (big endian).
    """
    s = message.SerializeToString()
    packed_len = struct.pack('>L', len(s))
    sock.sendall(packed_len + s)

def get_message(sock, msgtype):
    """ Read a message from a socket. msgtype is a subclass of
        of protobuf Message.
    """
    len_buf = socket_read_n(sock, 4)
    msg_len = struct.unpack('>L', len_buf)[0]
    msg_buf = socket_read_n(sock, msg_len)

    msg = msgtype()
    msg.ParseFromString(msg_buf)
    return msg, msg_len

def socket_read_n(sock, n):
    """ Read exactly n bytes from the socket.
        Raise RuntimeError if the connection closed before
        n bytes were read.
    """
    buf = ''
    while n > 0:
        data = sock.recv(n)
        if data == '':
            raise RuntimeError('unexpected connection close')
        buf += data
        n -= len(data)
    return buf

def packIntegerAsULong(value):
        """Packs a python 4 byte unsigned integer to an arduino unsigned long"""
        return struct.pack('I', value)    #should check bounds

def main():
    HOST = ''   # Symbolic name, meaning all available interfaces
    PORT = 8888 # Arbitrary non-privileged port

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    print 'Socket created'

    #Bind socket to local host and port
    try:
        s.bind((HOST, PORT))
    except socket.error as msg:
        print 'Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
        sys.exit()

    print 'Socket bind complete'

    #Start listening on socket
    s.listen(10)
    print 'Socket now listening'

    #wait to accept a connection - blocking call
    conn, addr = s.accept()
    print 'Connected with ' + addr[0] + ':' + str(addr[1])
    ser = serial.Serial('/dev/tty.usbmodem1421', 115200, timeout=0)
    print ser.readline()
    #now keep talking with the client
    while 1:
        #print "Message recevied."
        controller_state, msg_len = get_message(conn, controller_state_pb2.ControllerState)
        binary_string = controller_state.SerializeToString()
        ser.write('' + chr(len(binary_string)))
        ser.write(binary_string)
        msg = ser.readline()
        if "failure" in msg:
            print "resync" + msg
            ser.flushOutput()
            ser.write("A")
        else:
            print msg


    s.close()

if __name__ == '__main__':
    main()
