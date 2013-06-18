TEMPLATE = subdirs
SUBDIRS = sub_src sub_examples \
    generator
CONFIG += ordered

sub_src.subdir = src

sub_examples.subdir = examples
