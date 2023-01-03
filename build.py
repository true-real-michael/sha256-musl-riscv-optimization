#!/usr/bin/env python3

import argparse
import os
import subprocess


def parse_args():
    parser = argparse.ArgumentParser(description="Build modified musl")
    parser.add_argument('--musldir',
                        required=True,
                        help='musl sources directory')
    parser.add_argument('--prefix',
                        default='./build',
                        help='where the build will be located, default: ./build')
    parser.add_argument('--unrolling',
                        action='store_true',
                        help='use compression loop unrolling of SHA-256')
    parser.add_argument('--parallelization',
                        action='store_true',
                        help='use expansion loop parallelization of SHA-256')
    parser.add_argument('-O',
                        choices=[1, 2, 3, 's'],
                        help='how the source code of SHA-256 will be optimized',
                        default='s')
    return parser.parse_args()


def build_musl(args, musldir, prefix):
    subprocess.run(
        ['riscv64-linux-gnu-gcc',
         '-S',
         f'-O{args.O}',
         f'modified_sha/crypt_sha256_musl_optim{"1" if args.parallelization else ""}{"2" if args.unrolling else ""}.c',
         '-o',
         'crypt_sha256.s'], check=True)
    if not os.path.isdir(musldir + '/src/crypt/riscv64'):
        subprocess.run(['mkdir', musldir + '/src/crypt/riscv64'], check=True)
    subprocess.run(['mv', 'crypt_sha256.s', f'{musldir}/src/crypt/riscv64'], check=True)

    subprocess.run(['./configure', f'--prefix={prefix}', f'--syslibdir={prefix}/lib', '--host=riscv64-linux-gnu'],
                   cwd=musldir, check=True)

    subprocess.run(['make', 'clean'], cwd=musldir, check=True)
    subprocess.run(['make', '-j8'], cwd=musldir, check=True)
    subprocess.run(['make', '-j8', 'install'], cwd=musldir, check=True)

    subprocess.run(['rm', musldir + '/src/crypt/riscv64/crypt_sha256.s'], check=True)


def main():
    args = parse_args()

    musldir = os.path.abspath(args.musldir)
    prefix = os.path.abspath(args.prefix)

    build_musl(args, musldir, prefix)


if __name__ == '__main__':
    main()
