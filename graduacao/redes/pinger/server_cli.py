import sys
import argparse
from server import Server, UDPServer


if __name__ == '__main__':
    # arguments
    parser = argparse.ArgumentParser(
        usage="%(prog)s [OPTION]...",
        description=("Run a server to receive ping and respond " "with pong to compute rtt."),
    )
    parser.add_argument(
        "-ip",
        "--server_ip",
        help=('Set the IP address that the server will use. Default is 127.0.0.1.'),
        action='store',
        default='127.0.0.1',
    )
    parser.add_argument(
        "-port",
        "--server_port",
        help=('Set the port that the server will use. Default is 3000.'),
        action='store',
        default=3000,
    )
    parser.add_argument(
        "-l",
        "--logger",
        help='Save all server output in log file.',
        action='store_true',
    )
    parser.add_argument(
        "-t",
        "--timeout",
        help=('Set how long to wait for a client to receive a ' 'response before receiving a timeout, in seconds.'),
        action='store',
        default=10,
    )
    parser.add_argument(
        "-sd",
        "--simulate_delay",
        help=('Simulate delay in server-side with minimum 10ms and maximum 200ms.'),
        action='store_true',
    )
    parser.add_argument(
        "-sl",
        "--simulate_loss",
        help=('Simulate server-side loss with 25%% of chance.'),
        action='store_true',
    )
    parser.add_argument(
        "-sp",
        "--simulate_protocol_error",
        help=('Simulate server-side protocol errors with 25%% of chance.'),
        action='store_true',
    )
    args = parser.parse_args()

    # stdout change
    if args.logger:
        previous = sys.stdout
        sys.stdout = open('server_log.txt', 'w', encoding='utf8')

    # server run
    server: Server = UDPServer(timeout=int(args.timeout))

    # settings
    if args.simulate_delay:
        server.set_setting('simulate_delay', True)

    if args.simulate_loss:
        server.set_setting('simulate_loss', True)

    if args.simulate_protocol_error:
        server.set_setting('simulate_protocol_error', True)

    server.emmit_setting()
    server.connect(
        server_ip=args.server_ip,
        server_port=int(args.server_port),
    )
    server.listen()

    # revert stdout change
    if args.logger:
        sys.stdout.close()
        sys.stdout = previous
