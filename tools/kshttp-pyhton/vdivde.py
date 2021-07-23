"""
Example for using the kshttp-python functions
In this example we change the times of videvde3696 blocks to account for different behaviour when switching to a
build with fixed tasklist patch.
This only works if all relevant vdivde3696 blocks run with the cycle time of the UrTask.
"""

from kshttp import *


def fix_vdivde(server_name: str = 'MANAGER', host: str = 'localhost', max_time: float = 30):
    """Fix some vdivde3696 blocks with new build

    :param str server_name: name of server to fix
    :param str host: host of server
    :param float max_time: maximal time value of vdivde3696 blocks to decide if they need fixing
    :return: None
    """
    s = server_by_name(server_name, host=host)

    # fix vdivde3696 timer
    if object_exists(s, '/acplt/vdivde3696'):
        time_blocks = list()
        fix_flag = False

        # get instances of timed blocks
        for cls in ['/acplt/vdivde3696/' + x for x in ('ton1', 'tof1', 'tp1')]:
            time_blocks.extend(get_instances(s, cls))

        times = list()

        # get list of times
        for b in time_blocks:
            times.append(float(s.get_var(b + '.time')[0]))

        # get base cycle time
        cyctime = float(s.get_var('/Tasks/UrTask.cyctime')[0])

        # correct if over max_time
        if max(times) > max_time:
            fix_flag = True
            for b, t in zip(time_blocks, times):
                s.set_var(b + '.time', t * cyctime)

        # fix dead time and average block
        if fix_flag:
            for b in get_instances(s, '/acplt/vdivde3696/deadt'):
                val = float(s.get_var(b + '.dt')[0])
                s.set_var(b + '.dt', val * cyctime)

            for b in get_instances(s, '/acplt/vdivde3696/aver'):
                val = float(s.get_var(b + '.ta')[0])
                s.set_var(b + '.ta', val * cyctime)


if __name__ == '__main__':
    fix_vdivde('MANAGER')
