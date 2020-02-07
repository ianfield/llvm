// RUN: %clang_cc1 -verify -fsyntax-only -fsycl-is-device %s

void usage() {
  // expected-error@+1{{SYCL kernel cannot use a non-const static data variable}}
  static int m; 
}

template <typename Name, typename Func>
__attribute__((sycl_kernel)) void kernel_single_task(Func kernelFunc) {
  // expected-error@+1{{SYCL kernel cannot use a non-const static data variable}}
  static int z;
  // expected-note-re@+2{{called by 'kernel_single_task<fake_kernel, (lambda at {{.*}})>}}
  // expected-note-re@+1{{called by 'kernel_single_task<fake_kernel, (lambda at {{.*}})>}}
  kernelFunc();
}

int main() {
  static int n;
  kernel_single_task<class fake_kernel>([]() {
    //  expected-note@+1{{called by 'operator()'}}
    usage();
    // expected-error@+1{{SYCL kernel cannot use a non-const static data variable}}
    static int g;
  });

  return 0;
}
