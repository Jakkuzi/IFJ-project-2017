import os
import sys
import subprocess


if __name__ == '__main__':
    local_dir = os.popen('pwd').read()
    app = local_dir[:-1]+'/'+sys.argv[1]

    test_files = os.popen('find tests/ -regex ".*test_.*"').read()
    test_files = test_files.split('\n')
    test_files.pop()

    wrong = 0
    good = 0

    for file in test_files:
        test_name = file.split('/')
        success_return = file[-1]
        ret_code = os.WEXITSTATUS(os.system(local_dir[:-1]+'/'+sys.argv[1] + ' < ' + local_dir[:-1]+'/'+file))
        if str(ret_code) != success_return:
            print('\x1b[0;37;41m' + 'Expected return code {}, got {}\x1b[0m --- {}'.format(success_return, ret_code,
                                                                                     test_name[-1]))
            wrong += 1
        else:
            print('\x1b[5;30;42m{}'.format(test_name[-1])+'\x1b[0m'+' --- passed')
            good += 1

    print ('\n\nSummary: {} tests passed, {} failed'.format(good, wrong))