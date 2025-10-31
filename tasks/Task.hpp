/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef IODRIVERS_BASE_TASK_TASK_HPP
#define IODRIVERS_BASE_TASK_TASK_HPP

#include "iodrivers_base/TaskBase.hpp"

namespace iodrivers_base {
    class Driver;
    class PortListener;
    class PortStream;
    class ConfigureGuard;

    /** Generic integration of an iodrivers_base::Driver in oroGen
     *
     * configureHook: Subclasses must call setDriver with a valid driver object
     * before calling the base task's configureHook. Any I/O must be done after
     * the base configureHook is done. If the _io_port property is not empty,
     * the subclass must open the URI itself. Otherwise, the base configureHook
     * will tie the task's io_raw_in and io_raw_out ports to the device. Additionally,
     * in order to easily guarantee an acceptable level of exception-safety, one
     * should use the scope guard ConfigureGuard provided with this package.
     *
     * <code>
     * bool Task::configureHook()
     * {
     *   iodrivers_base::ConfigureGuard guard(this);
     *   std::auto_ptr<MyDriver> driver(new MyDriver);
     *   if (!_io_port.get().empty())
     *     driver->openURI(_io_port.get());
     *   setDriver(driver.release());
     *
     *   if (!TaskBase::configureHook())
     *     return false;
     *
     *   // Do device initialization here. NEVER before
     *   // TaskBase::configureHook has been called
     *   // Also, don't forget to release the guard by
     *   // calling commit() on it just before returning.
     *   guard.commit();
     *   return true;
     * }
     * </code>
     *
     * cleanupHook: the base implementation closes the device. Perform device
     * de-initialization before calling the base cleanupHook.
     *
     * processIO: called when the task is called because of I/O has been
     * received. It will be called as long as there are valid packets buffered,
     * so processIO must ensure that all available packets have been read.
     *
     * <h2>Device ownership</h2>
     *
     * The device object is owned by the subclass, which is responsible for its
     * destruction. setDriver may be called more than once with the same device
     * (so, it's OK to allocate the device as an attribute instead of allocating
     * on the heap).
     *
     * Calling setDriver adds connections between the driver and task ports. If
     * this connection must be removed (to get a "detached" device), one must
     * call detachDriver() after having called setDriver. Note that one does not
     * need to do so before the driver is deleted. Calling detachDriver after
     * having deleted the driver, though, will lead to a use-after-free.
     */
    class Task : public TaskBase
    {
	friend class TaskBase;
    friend class ConfigureGuard;
        bool mRuntimeErrorIOProcessingEnabled;

    protected:
        Driver* mDriver;

        /** IOStream object that does I/O on the ports */
        PortStream* mStream;
        /** Listener object that forwards I/O to the monitoring ports */
        PortListener* mListener;

        RawPacket mRawPacket;
        base::Time mLastStatus;

        base::Time mIOWaitTimeout;
        base::Time mIOWaitDeadline;

        /** Sets the driver object
         *
         * Must be called before either the TaskBase::startHook method gets
         * called for tasks that do not require configuration, or
         * TaskBase::configureHook otherwise
         */
        void setDriver(Driver* driver);

        /** Detaches the current driver from the task
         */
        void detachDriver();

        /** Returns the current driver object
         */
        Driver* getDriver() const;

        /** DEPRECATED
         */
        void pushAllData();

        /** Returns true if there is some I/O available to read on the driver
         */
        bool hasIO();

        void processDriverState();

        void processPendingIO(void (Task::*callback)());

        /** Called back by the updateHook in case we have not received any data
         * for the duration of io_wait_timeout
         *
         * The default implementation transitions to IO_TIMEOUT
         */
        virtual void processIOTimeout();

        /** Called back by the updateHook. It must be reimplemented to process
         * all packets that are currently queued in the driver
         */
        virtual void processIO() = 0;

        /** Returns whether \c errorHook will perform I/O processing
         *
         * @see setRuntimeErrorIOProcessingEnabled
        */
        bool getRuntimeErrorIOProcessingEnabled() const;

        /** Changes whether \c errorHook will perform I/O processing
         *
         * The default is to NOT do any processing in errorHook, for historical
         * reasons. We recommend setting this to true if you use runtime error
         * states
         */
        void setRuntimeErrorIOProcessingEnabled(bool flag);

        /** Called back by the errorHook. It must be reimplemented to process
         * all packets that are currently queued in the driver
         */
        virtual void errorProcessIO();

        /** Updates the IO status information on the io_status port */
        void updateIOStatus();

        /** Configuration specific to a FileDescriptorActivity */
        void configureFDActivity();

        /** Configure the aperiodic trigger timeout for the activity*/
        void configureActivityAperiodicTimeout();

    public:
        Task(std::string const& name = "iodrivers_base::Task");
        Task(std::string const& name, RTT::ExecutionEngine* engine);
	virtual ~Task();

        /** This hook is called by Orocos when the state machine transitions
         * from PreOperational to Stopped. If it returns false, then the
         * component will stay in PreOperational. Otherwise, it goes into
         * Stopped.
         *
         * It is meaningful only if the #needs_configuration has been specified
         * in the task context definition with (for example):
         *
         *   task_context "TaskName" do
         *     needs_configuration
         *     ...
         *   end
         */
        bool configureHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to Running. If it returns false, then the component will
         * stay in Stopped. Otherwise, it goes into Running and updateHook()
         * will be called.
         */
        bool startHook();

        /** Handling of FD-related errors, timeout and I/O processing in runtime
         * state
         *
         * This calls processIO in the subclass
         */
        void updateHook();

        /** Handling of FD-related errors, timeout and I/O processing in runtime error
         * states
         *
         * This calls errorProcessIO in the subclass
         */
        void errorHook();

        void exceptionHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Running to Stopped after stop() has been called.
         */
        void stopHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to PreOperational, requiring the call to configureHook()
         * before calling start() again.
         */
        void cleanupHook();
    };
}

#endif

