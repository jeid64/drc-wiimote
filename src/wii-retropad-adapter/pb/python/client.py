import pygame
import time
import controller_state_pb2
import socket
import struct

pygame.init()
pygame.display.set_mode([854, 480], pygame.RESIZABLE)
pygame.display.set_caption("drc-sim")
done = False
pygame.joystick.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()

global sock

xbox = 0

def send_message(message):
    """ Send a serialized message (protobuf Message interface)
        to a socket, prepended by its length packed in 4
        bytes (big endian).
    """
    s = message.SerializeToString()
    packed_len = struct.pack('>L', len(s))
    sock.sendall(packed_len + s)

def hid_snd():
    global joystick, hid_seq_id
    controller_state = controller_state_pb2.ControllerState()
    controller_state.controller_id = 0
    button_mapping = {}
    if xbox == 1:
        xbox_button_mapping = {
            0 : "ba", # a
            1 : "bb", # b
            2 : "bx", # x
            3 : "by", # y
            4 : "bzl", # l
            5 : "bzr", # r
            6 : "bm", # back (minus)
            7 : "bp", # start (plus)
            # On windows, 8 isn't a button.
            #8 : 0x0002, # xbox (home)
            # extra buttons
            8 : "bl", # l3
            9 : "br"  # r3
        }
        button_mapping = xbox_button_mapping
        # hat: (<l/r>, <u/d>) [-1,1]
        hat = joystick.get_hat(0)
        if hat[0] is 0:
            controller_state.bdl = 0
            controller_state.bdr = 0
        elif hat[0] is -1:
            controller_state.bdl = 1
            controller_state.bdr = 0
        else:
            controller_state.bdl = 0
            controller_state.bdr = 1

        if hat[1] is 0:
            controller_state.bdd = 0
            controller_state.bdu = 0
        elif hat[1] is -1:
            controller_state.bdd = 1
            controller_state.bdu = 0
        else:
            controller_state.bdd = 0
            controller_state.bdu = 1
    else:
        ps3_button_mapping = {
            14 : "ba", # a
            13 : "bb", # b
            15 : "bx", # x
            12 : "by", # y
            10 : "bzl", # l
            11 : "bzr", # r
            0  : "bm", # back (minus)
            3  : "bp", # start (plus)
            16 : "bhome", # xbox (home)
            # extra buttons
            1 : "bl", # l3
            2 : "br",  # r3
            4 : "bdu",  # dpad up
            5 : "bdr",  # dpad right
            6 : "bdd",  # dpad down
            7 : "bdl"  # dpad left
        }
        button_mapping = ps3_button_mapping

    hat_mapping_x = {
        0 : 0x000,
       -1 : "bdl", # l
        1 : "bdr" # r
    }

    hat_mapping_y = {
        0 : 0x000,
       -1 : "bdd", # d
        1 : "bdu" # u
    }
    trigger_mapping = {
        2 : 0x0080, # l
        5 : 0x0040  # r
    }

    for key,value in button_mapping.iteritems():
        if joystick.get_button(key):
            button_mapping[key] = 1
            print("got button, " + str(key) + " value is " + str(button_mapping[key]))
            setattr(controller_state, value, 1)
        else:
            setattr(controller_state, value, 0)

    controller_state.lt = 0
    controller_state.rt = 0
    controller_state.lx = 0
    controller_state.ly = 0
    controller_state.rx = 0
    controller_state.ry = 0
    #button_bits |= hat_mapping_x[hat[0]]
    #button_bits |= hat_mapping_y[hat[1]]
    # xbox 360 axis.
    # LX, LY, RX, RY
    # 0: l stick l/r
    # 1: l stick u/d
    # 2: l trigger
    # 3: r stick l/r
    # 4: r stick u/d
    # 5: r trigger

    #ps3 axis
    # 0: l stick l/r
    # 1: l stick u/d
    # 5: l trigger
    # 2: r stick l/r
    # 3: r stick u/d
    # 6: r trigger
    # don't know what four is.
    #for i in xrange(4):
    #    if i in (5, 6):
    #        if joystick.get_axis(i) > 0:
    #            button_bits |= trigger_mapping[i]
    #    else:
    #        orig = joystick.get_axis(i)
    #        scaled = 0x800
    #        if abs(orig) > 0.2:
    #            if i in (0, 2):
    #                scaled = scale_stick(orig, -1, 1, 900, 3200)
    #            elif i in (1, 3):
    #                scaled = scale_stick(orig, 1, -1, 900, 3200)
    #        #print '%04i %04i %f' % (i, scaled, orig)
    #        stick_mapping = { 0 : 0, 1 : 1, 2 : 2, 3 : 3 }
    #        report[3 + stick_mapping[i]] = scaled
    #report[1] = (button_bits >> 8) | ((button_bits & 0xff) << 8)
    send_message(controller_state)


sock = socket.socket()
host = socket.gethostname()
port = 8888

sock.connect((host, port))

EVT_SEND_HID = pygame.USEREVENT
pygame.time.set_timer(EVT_SEND_HID, int((1. / 180.) * 1000.))

while not done:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
        elif event.type == pygame.VIDEORESIZE:
            pygame.display.set_mode(event.size, pygame.RESIZABLE)
        elif event.type == EVT_SEND_HID:
            hid_snd()

sock.close()
pygame.quit()
